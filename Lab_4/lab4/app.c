/*
 * task 2.1, 2.2, 2.3
 *
 *  Created on: 21 Nov 2020
 *      Author: Kyo
 */

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
#include <stdio.h>
#include <math.h>

#include <FreeRTOS.h>
#include <task.h>

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <bosch/bmi160/bmi160.h>

#include "gpio.h"
#include "sensor.h"
#include "uart.h"
#include "app.h"


/**
 * The acceleration calibration application state to store the zero offset of the sensor.
 */
volatile struct bmi160_sensor_data calibration_acc = {
  .sensortime = 0,
  .x = 0,
  .y = 0,
  .z = 0,
};

/**
 * Flag to request use of next sensor value as calibration.
 */
volatile uint_fast8_t calibration_flag = 0;


/**
 * Task 2.1, 2.2 and 2.3
 *
 * @todo Implement your processing task from scratch.
 * The task should wait for new sensor data, calculate the attitude of the
 * board (tilt angel in x-, y-direction) and forward them to the Print and
 * LED queues if is has changed significantly (i.e. absolute angle change in
 * degrees greater or equal to appANGLE_SIGINFICANT_CHANGE value).
 *
 * Hint:
 * Use the `processing_task_param_t`, `sensor_data_t`, and `attitude_data_t`
 * structures defined in the app.h header file for task arguments, sensor
 * data input and attitude output data, respectively.
 */

/**
 * Sensor data processing task.
 * @param pvParameters Task parameters
 */

//m2_3s//
void prvProcessingTask(void* pvParameters)
{
  // Data structures
  sensor_data_t sensor_data;
  attitude_data_t attitude_data;
  processing_task_param_t param;

  // Get the task parameters
  param = *((processing_task_param_t*)pvParameters);

  // Low level init
  // --- not required ---

  // Print task start
  uart_println("Processing task starting...");

  // Comparison variables (invalid angle value to update on first run)
  attitude_data_t attitude_last = {
    .theta_x = -1000, .theta_y = -1000, .time = 0,
  };

  // Enter main sensing loop
  while (1)
  {
    // Wait for new sensor data to be sent
    xQueueReceive(param.inDataQueue, (void*)&sensor_data, portMAX_DELAY);

    // Offset compensation
    float acc_x = (float)(sensor_data.acc_x - calibration_acc.x);
    float acc_y = (float)(sensor_data.acc_y - calibration_acc.y);
    float acc_z = (float)(sensor_data.acc_z - calibration_acc.z);

    // Basic angle calculation for gravity vector
    float norm = sqrtf(acc_x*acc_x + acc_y*acc_y + acc_z*acc_z);
    float theta_x = asinf(acc_x/norm); // in radian
    float theta_y = asinf(acc_y/norm); // in radian

    attitude_data.theta_x = 180 * theta_x / PI; // in degree
    attitude_data.theta_y = 180 * theta_y / PI; // in degree
    attitude_data.time = sensor_data.time;

    // Check if angles changed more than specified value
    if (abs(attitude_data.theta_x - attitude_last.theta_x) >= appANGLE_SIGNIFICANT_CHANGE ||
        abs(attitude_data.theta_y - attitude_last.theta_y) >= appANGLE_SIGNIFICANT_CHANGE)
    {
      // Send updated value
      xQueueSendToBack(param.outLedQueue, (void*)&attitude_data, 0);
      xQueueSendToBack(param.outPrintQueue, (void*)&attitude_data, 0);
      attitude_last = attitude_data;
    }
  }

  // Delete task before reaching end
  vTaskDelete(NULL);
}
//m2_3e//
/* ------------------------------------------------------------------------- */



/**
 * Motion sensor readout task.
 * @param pvParameters Task parameters
 */
void prvSensorTask(void* pvParameters)
{
  // Sensor and data structures
  struct bmi160_dev bmi160_sensor;
  struct bmi160_sensor_data data_accel = {0};
  struct bmi160_sensor_data data_gyro = {0};
  sensor_data_t sensor_data;
  sensor_task_param_t param;

  // Get task parameters
  param = *((sensor_task_param_t*)pvParameters);

  // Init BMI160 sensor
  uint_fast8_t init_result = sensor_init(&bmi160_sensor);
  if (init_result > 0)
  {
    uart_println("Error %i initializing BMI160.", init_result);
  }

  // Print task start
  uart_println("Sensor task starting...");

  // --- main sensing loop ---
  while (1)
  {
    // Wait for new data (polling INT1 interrupt signal)
    while (GPIO_getInputPinValue(SENSOR_INT1_PORT, SENSOR_INT1_PIN) != GPIO_INPUT_PIN_HIGH)
    {
      vTaskDelay(pdMS_TO_TICKS(0.1));
    }

    // Read data: acc, gyro and time
    int8_t result =
      bmi160_get_sensor_data((BMI160_ACCEL_SEL | BMI160_GYRO_SEL | BMI160_TIME_SEL),
                             &data_accel,
                             &data_gyro,
                             &bmi160_sensor);
    if (result != BMI160_OK)
    {
      uart_println("Error reading data from sensor.");
    }

    // If calibration requested, store raw data and skip forwarding to queue
    if (calibration_flag > 0)
    {
      // Copy current sensor values, with compensation of z-axis using value for 1g
      calibration_acc.sensortime = data_accel.sensortime;
      calibration_acc.x = data_accel.x;
      calibration_acc.y = data_accel.y;
      calibration_acc.z = data_accel.z - sensorACC_1G;
      calibration_flag = 0;
      continue;
    }

    // Assemble data structure to send to the queue
    sensor_data.time = data_accel.sensortime;
    sensor_data.acc_x = data_accel.x;
    sensor_data.acc_y = data_accel.y;
    sensor_data.acc_z = data_accel.z;
    sensor_data.gyro_x = data_gyro.x;
    sensor_data.gyro_y = data_gyro.y;
    sensor_data.gyro_z = data_gyro.z;

    // Send to queue, skip if full
    xQueueSendToBack(param.outDataQueue, (void*)&sensor_data, (TickType_t)0);

    // Send to queue, block if full
    //xQueueSendToBack(param.outDataQueue, (void* )&sensor_data, portMAX_DELAY);
  }

  /*
   * Should the task implementation ever break out of the above loop, then the task
   * must be deleted before reaching the end of its implementing function. The NULL
   * parameter passed to the vTaskDelete() API function indicates that the task to be
   * deleted is the calling (this) task. The convention used to name API functions is
   * described in section 1.5, Data Types and Coding Style Guide.
   */
  vTaskDelete(NULL);
}
/* ------------------------------------------------------------------------- */


/**
 * Print data to UART task.
 * @param pvParameters Task parameters
 */
void prvPrintTask(void* pvParameters)
{
  // Data structures
  attitude_data_t attitude_data;
  print_task_param_t param;

  // Get task parameters
  param = *((print_task_param_t*)pvParameters);

  // Low level init
  // --- UART initialized globally ---

  // Print task start
  uart_println("Print task starting...");

  // Enter main sensing loop
  while (1)
  {
    // Wait for new data to be sent
    xQueueReceive(param.inDataQueue, (void*)&attitude_data, portMAX_DELAY);

    // Indicate data received
    GPIO_setOutputHighOnPin(LED1_PORT, LED1_PIN);

    // Send via UART
    uart_println("time:%9lu  --  theta_x: % 6i   theta_y: % 6i",
                 attitude_data.time,
                 (int32_t)(1e3 * attitude_data.theta_x),
                 (int32_t)(1e3 * attitude_data.theta_y));
  }

  // Delete task before reaching end
  vTaskDelete(NULL);
}
/* ------------------------------------------------------------------------- */


/**
 * Dangerous attitude indicator using LED task.
 * @param pvParameters Task parameters
 */
void prvLedTask(void* pvParameters)
{
  // Data structures
  attitude_data_t attitude_data;
  led_task_param_t param;

  // Get task parameters
  param = *((led_task_param_t*)pvParameters);

  // Low level init
  // --- UART and GPIO initialized globally ---

  // Print task start
  uart_println("LED task starting...");

  // Enter main sensing loop
  while (1)
  {
    // Wait for new data, blocking read
    xQueueReceive(param.inDataQueue, (void*)&attitude_data, portMAX_DELAY);

    if (abs(attitude_data.theta_x) > appWARNING_ANGLE_DEG ||
        abs(attitude_data.theta_y) > appWARNING_ANGLE_DEG)
    {
      GPIO_setOutputHighOnPin(LED2_RED_PORT, LED2_RED_PIN);
      GPIO_setOutputLowOnPin(LED2_GREEN_PORT, LED2_GREEN_PIN);
      GPIO_setOutputLowOnPin(LED2_BLUE_PORT, LED2_BLUE_PIN);
    }
    else
    {
      GPIO_setOutputHighOnPin(LED2_GREEN_PORT, LED2_GREEN_PIN);
      GPIO_setOutputLowOnPin(LED2_RED_PORT, LED2_RED_PIN);
      GPIO_setOutputLowOnPin(LED2_BLUE_PORT, LED2_BLUE_PIN);
    }
  }

  // Delete task before reaching end
  vTaskDelete(NULL);
}
/* ------------------------------------------------------------------------- */


void vPreSleepProcessing(uint32_t ulExpectedIdleTime)
{
}
/* ------------------------------------------------------------------------- */


void vApplicationTickHook(void)
{
  /* This function will be called by each tick interrupt if
   configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
   added here, but the tick hook is called from an interrupt context, so
   code must not attempt to block, and only the interrupt safe FreeRTOS API
   functions can be used (those that end in FromISR()). */

  /* The full demo includes a software timer demo/test that requires
   prodding periodically from the tick interrupt. */
  // vTimerPeriodicISRTests();
  /* Call the periodic event group from ISR demo. */
  // vPeriodicEventGroupsProcessing();
  /* Use task notifications from an interrupt. */
  // xNotifyTaskFromISR();
  /* Use mutexes from interrupts. */
  // vInterruptSemaphorePeriodicTest();
}
/* ------------------------------------------------------------------------- */
