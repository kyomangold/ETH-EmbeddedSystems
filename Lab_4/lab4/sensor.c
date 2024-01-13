/*
 * Copyright (c) 2018, Swiss Federal Institute of Technology (ETH Zurich).
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Author:  Lukas Sigrist
 */

#include <stdint.h>

#include <FreeRTOS.h>
#include <task.h>

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <bosch/bmi160/bmi160.h>

#include "sensor.h"


/**
 * Sensor I2C interface configuration (with source clock frequency placeholder)
 */
eUSCI_I2C_MasterConfig sensor_i2cConfig = {
  .selectClockSource = EUSCI_B_I2C_CLOCKSOURCE_SMCLK,
  .i2cClk = 0,
  .dataRate = EUSCI_B_I2C_SET_DATA_RATE_1MBPS,
  .byteCounterThreshold = 0,
  .autoSTOPGeneration = EUSCI_B_I2C_NO_AUTO_STOP,
};


/**
 * Initialize the sensor and the low-level peripherals used to access it.
 * @return Zero on success, non-zero error code on failure.
 */
uint_fast8_t sensor_init(struct bmi160_dev* bmi160_sensor)
{
  // initialize GPIO first
  sensor_init_gpio();

  // I2C master configuration
  sensor_i2cConfig.i2cClk = CS_getSMCLK();
  I2C_initMaster(SENSOR_I2C_INTERFACE, &sensor_i2cConfig);

  // interrupt setup
  // -- none --

  // enable I2C interface
  I2C_enableModule(SENSOR_I2C_INTERFACE);

  // configure off-chip sensor
  bmi160_sensor->id        = BMI160_I2C_ALT_ADDR;
  bmi160_sensor->interface = BMI160_I2C_INTF;
  bmi160_sensor->read      = sensor_i2c_read;
  bmi160_sensor->write     = sensor_i2c_write;
  bmi160_sensor->delay_ms  = sensor_delay_ms;

  int_fast8_t result = BMI160_OK;
  result = bmi160_init(bmi160_sensor);
  if (result != BMI160_OK)
  {
    return SENSOR_INIT_ERROR;
  }

  // init BMM150 sensor as slave of BMI160
  // -- skipped for now --

  // configure sensing of BMI160
  // select the Output data rate, range of accelerometer sensor
  bmi160_sensor->accel_cfg.odr   = BMI160_ACCEL_ODR_1600HZ;
  bmi160_sensor->accel_cfg.range = BMI160_ACCEL_RANGE_2G;
  bmi160_sensor->accel_cfg.bw    = BMI160_ACCEL_BW_NORMAL_AVG4;

  // select the power mode of accelerometer sensor */
  bmi160_sensor->accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;

  // select the Output data rate, range of Gyroscope sensor
  bmi160_sensor->gyro_cfg.odr    = BMI160_GYRO_ODR_3200HZ;
  bmi160_sensor->gyro_cfg.range  = BMI160_GYRO_RANGE_250_DPS;
  bmi160_sensor->gyro_cfg.bw     = BMI160_GYRO_BW_NORMAL_MODE;

  // select the power mode of Gyroscope sensor
  bmi160_sensor->gyro_cfg.power  = BMI160_GYRO_NORMAL_MODE;

  // set the sensor configuration
  result = bmi160_set_sens_conf(bmi160_sensor);
  if (result != BMI160_OK)
  {
    return SENSOR_CONFIG_ERROR;
  }

  // configure BMI160 data ready interrupt
  struct bmi160_int_settg bme160_int1_config;

  // select the Interrupt channel/ pin and type
  bme160_int1_config.int_channel = BMI160_INT_CHANNEL_1;
  bme160_int1_config.int_type = BMI160_ACC_GYRO_DATA_RDY_INT;

  // select the interrupt channel/pin settings
  bme160_int1_config.int_pin_settg.output_en = BMI160_ENABLE;     // Enabling interrupt pins 
                                                                  // to act as output pin
  bme160_int1_config.int_pin_settg.output_mode = BMI160_DISABLE;  // Choosing push-pull mode
                                                                  // for interrupt pin
  bme160_int1_config.int_pin_settg.output_type = BMI160_ENABLE;   // Choosing active high
                                                                  // output
  bme160_int1_config.int_pin_settg.edge_ctrl = BMI160_DISABLE;    // Choosing edge triggered
                                                                  // output
  bme160_int1_config.int_pin_settg.input_en = BMI160_DISABLE;     // Disabling interrupt pin 
                                                                  // to act as input
  bme160_int1_config.int_pin_settg.latch_dur = BMI160_LATCH_DUR_NONE;  // non-latched output

  // set the flat interrupt configuration
  result = bmi160_set_int_config(&bme160_int1_config, bmi160_sensor);
  if (result != BMI160_OK)
  {
    return SENSOR_INTERUPT_ERROR;
  }

  // --- configure BMI160 data ready interrupt ---
  struct bmi160_int_settg bme160_int2_config;

  // select the Interrupt channel/ pin and type
  bme160_int2_config.int_channel = BMI160_INT_CHANNEL_2;
  bme160_int2_config.int_type = BMI160_ACC_DOUBLE_TAP_INT;

  // select the interrupt channel/pin settings
  bme160_int2_config.int_pin_settg.output_en = BMI160_ENABLE;     // Enabling interrupt pins 
                                                                  // to act as output pin
  bme160_int2_config.int_pin_settg.output_mode = BMI160_DISABLE;  // Choosing push-pull mode
                                                                  // for interrupt pin
  bme160_int2_config.int_pin_settg.output_type = BMI160_ENABLE;   // Choosing active high
                                                                  // output
  bme160_int2_config.int_pin_settg.edge_ctrl = BMI160_DISABLE;    // Choosing edge triggered
                                                                  // output
  bme160_int2_config.int_pin_settg.input_en = BMI160_DISABLE;     // Disabling interrupt pin
                                                                  // to act as input
  bme160_int2_config.int_pin_settg.latch_dur =
    BMI160_LATCH_DUR_40_MILLI_SEC;  // enable 40ms latched output

  // Select the Step Detector interrupt parameters, Kindly use the recommended settings for
  // step detector: . (x+1)* 62.5mg (2g), *125mg (4g), *250mg (8g), *500mg (16g)
  bme160_int2_config.int_type_cfg.acc_tap_int.tap_thr   = 20;
  bme160_int2_config.int_type_cfg.acc_tap_int.tap_shock = 0;     // 0 -> 50ms, 1 -> 75ms
  bme160_int2_config.int_type_cfg.acc_tap_int.tap_quiet = 0;     // 0 -> 30ms, 1 -> 25ms
  bme160_int2_config.int_type_cfg.acc_tap_int.tap_dur   = 0x4;   // 0b100 -> 250ms
  bme160_int2_config.int_type_cfg.acc_tap_int.tap_data_src = 1;  // data source 0- filter & 1
                                                                 // pre-filter
  bme160_int2_config.int_type_cfg.acc_tap_int.tap_en = BMI160_ENABLE;  // 1-enable, 0-disable
                                                                       // the tap detector

  // set the flat interrupt configuration
  result = bmi160_set_int_config(&bme160_int2_config, bmi160_sensor);
  if (result != BMI160_OK)
  {
    return SENSOR_INTERUPT_ERROR;
  }

  // zero return value on success
  return 0;
}
/* ------------------------------------------------------------------------- */


/**
 * Initialize the GPIO pins used for interfacing the sensors.
 */
void sensor_init_gpio(void)
{
  // configure clock and data ports to connect to I2C peripheral
  GPIO_setAsPeripheralModuleFunctionOutputPin(
    SENSOR_I2C_SCL_PORT, SENSOR_I2C_SCL_PIN, SENSOR_I2C_GPIO_AF);
  GPIO_setAsPeripheralModuleFunctionOutputPin(
    SENSOR_I2C_SDA_PORT, SENSOR_I2C_SDA_PIN, SENSOR_I2C_GPIO_AF);

  // interrupt lines as inputs
  GPIO_setAsInputPin(SENSOR_INT1_PORT, SENSOR_INT1_PIN);
  GPIO_setAsInputPin(SENSOR_INT2_PORT, SENSOR_INT2_PIN);
  GPIO_setAsInputPin(SENSOR_MAGINT_PORT, SENSOR_MAGINT_PIN);

  // interrupt setup
  // -- none --
}
/* ------------------------------------------------------------------------- */


/**
 * Read data from one or multiple sensor register(s).
 * @param device_address The sensors I2C bus address
 * @param register_address The sensors register address to start reading from
 * @param data Pointer to the memory where the received data is stored
 * @param count Number of bytes to read
 * @return Status code, zero on success, non-zero in case of errors
 */
int8_t sensor_i2c_read(uint8_t device_address,
                       uint8_t register_address,
                       uint8_t* data,
                       uint16_t count)
{
  /*
   * Data on the bus should be like
   * |------------+---------------------|
   * | I2C action | Data                |
   * |------------+---------------------|
   * | Start      | [device_address]    |
   * | Write      | (register_address)  |
   * | Start      | [device_address]    |
   * | Read       | (data[0])           |
   * | Read       | (....)              |
   * | Read       | (data[count - 1])   |
   * | Stop       | -                   |
   * |------------+---------------------|
   *
   * return 0 for success, non-zero for failure
   */

  // wait for I2C to be ready to read
  while (I2C_isBusBusy(SENSOR_I2C_INTERFACE))
  {
    continue;
  }

  // configure slave address
  I2C_setSlaveAddress(SENSOR_I2C_INTERFACE, device_address);

  // initiate read with START and writing register address
  I2C_masterSendMultiByteStart(SENSOR_I2C_INTERFACE, register_address);

  // wait for transmission to complete, clear interrupt flag
  while (!I2C_getInterruptStatus(SENSOR_I2C_INTERFACE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0))
  {
    continue;
  }
  I2C_clearInterruptFlag(SENSOR_I2C_INTERFACE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

  // check for NACK, on NACK stop and exit with failure
  if (I2C_getInterruptStatus(SENSOR_I2C_INTERFACE, EUSCI_B_I2C_NAK_INTERRUPT))
  {
    I2C_masterSendMultiByteStop(SENSOR_I2C_INTERFACE);
    return -1;
  }

  // start receive transaction with RE-START
  I2C_masterReceiveStart(SENSOR_I2C_INTERFACE);

  // wait for RE-START to complete send
  while (I2C_masterIsStartSent(SENSOR_I2C_INTERFACE))
  {
    continue;
  }

  // receive all but last data byte
  while (count > 1)
  {
    // wait for next byte transmission to complete
    while (!I2C_getInterruptStatus(SENSOR_I2C_INTERFACE, EUSCI_B_I2C_RECEIVE_INTERRUPT0))
    {
      continue;
    }

    // read received byte, clears RX interrupt automatically
    *data = I2C_masterReceiveMultiByteNext(SENSOR_I2C_INTERFACE);
    data = data + 1;

    count = count - 1;
  }

  // --- broken driverlib implementation to handle reception of last byte and STOPing ---
  // *data = I2C_masterReceiveMultiByteFinish(SENSOR_I2C_INTERFACE);
  // return 0;
  // --- alternative handling using driverlib API below ---

  // receive last byte and generate stop condition
  I2C_masterReceiveMultiByteStop(SENSOR_I2C_INTERFACE);

  // wait for stop interrupt
  while (!I2C_getInterruptStatus(SENSOR_I2C_INTERFACE, EUSCI_B_I2C_STOP_INTERRUPT))
  {
    continue;
  }

  // wait for next byte transmission to complete
  while (!I2C_getInterruptStatus(SENSOR_I2C_INTERFACE, EUSCI_B_I2C_RECEIVE_INTERRUPT0))
  {
    continue;
  }

  // actually read last received value
  *data = I2C_masterReceiveMultiByteNext(SENSOR_I2C_INTERFACE);

  return 0;
}
/* ------------------------------------------------------------------------- */


/**
 * Write data to one or multiple sensor register(s).
 * @param device_address The sensors I2C bus address
 * @param register_address The sensors register address to write
 * @param data Pointer to the data to be written
 * @param count Number of bytes to write
 * @return Status code, zero on success, non-zero in case of errors
 */
int8_t sensor_i2c_write(uint8_t device_address,
                        uint8_t register_address,
                        uint8_t* data,
                        uint16_t count)
{
  /*
   * Data on the bus should be like
   * |------------+---------------------|
   * | I2C action | Data                |
   * |------------+---------------------|
   * | Start      | [device_address]    |
   * | Write      | (register_address)  |
   * | Write      | (data[0])           |
   * | Write      | (...)               |
   * | Write      | (data[count - 1])   |
   * | Stop       | -                   |
   * |------------+---------------------|
   *
   * return 0 for success, non-zero for failure
   */

  // wait for I2C to be ready to write
  while (I2C_isBusBusy(SENSOR_I2C_INTERFACE))
  {
    continue;
  }

  // configure slave address
  I2C_setSlaveAddress(SENSOR_I2C_INTERFACE, device_address);

  // initiate read with START and writing register address
  I2C_masterSendMultiByteStart(SENSOR_I2C_INTERFACE, register_address);

  // wait for transmission to complete
  while (!I2C_getInterruptStatus(SENSOR_I2C_INTERFACE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0))
  {
    continue;
  }

  // check for NACK, on NACK stop and exit with failure
  if (I2C_getInterruptStatus(SENSOR_I2C_INTERFACE, EUSCI_B_I2C_NAK_INTERRUPT))
  {
    I2C_masterSendMultiByteStop(SENSOR_I2C_INTERFACE);
    return -1;
  }

  // send data
  while (count > 0)
  {
    // wait for next byte transmission to complete
    while (!I2C_getInterruptStatus(SENSOR_I2C_INTERFACE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0))
    {
      continue;
    }

    // send next byte, clears TX interrupt automatically
    I2C_masterSendMultiByteNext(SENSOR_I2C_INTERFACE, *data);
    data = data + 1;

    count = count - 1;
  }

  // wait for last transmission to complete, send STOP and finally clear flag
  while (!I2C_getInterruptStatus(SENSOR_I2C_INTERFACE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0))
  {
    continue;
  }
  I2C_masterSendMultiByteStop(SENSOR_I2C_INTERFACE);
  I2C_clearInterruptFlag(SENSOR_I2C_INTERFACE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

  return 0;
}
/* ------------------------------------------------------------------------- */


/**
 * Blocking delay for sensor initialization
 * @param period Delay in ms for which the task is blocked
 */
void sensor_delay_ms(uint32_t period)
{
  vTaskDelay(pdMS_TO_TICKS(period));
}
/* ------------------------------------------------------------------------- */
