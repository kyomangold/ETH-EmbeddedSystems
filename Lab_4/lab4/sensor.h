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

#ifndef SENSOR_H_
#define SENSOR_H_

#include <stdint.h>

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <bosch/bmi160/bmi160.h>


#define SENSOR_I2C_INTERFACE    EUSCI_B1_BASE
#define SENSOR_I2C_TIMEOUT      ((uint32_t) 10000)

#define SENSOR_I2C_SCL_PORT     GPIO_PORT_P6
#define SENSOR_I2C_SCL_PIN      GPIO_PIN5
#define SENSOR_I2C_SDA_PORT     GPIO_PORT_P6
#define SENSOR_I2C_SDA_PIN      GPIO_PIN4
#define SENSOR_I2C_GPIO_AF      GPIO_PRIMARY_MODULE_FUNCTION

#define SENSOR_INT1_PORT        GPIO_PORT_P4
#define SENSOR_INT1_PIN         GPIO_PIN1
#define SENSOR_INT2_PORT        GPIO_PORT_P5
#define SENSOR_INT2_PIN         GPIO_PIN0
#define SENSOR_MAGINT_PORT      GPIO_PORT_P4
#define SENSOR_MAGINT_PIN       GPIO_PIN6

#define SENSOR_INIT_ERROR       1
#define SENSOR_CONFIG_ERROR     2
#define SENSOR_INTERUPT_ERROR   3

#define sensorACC_1G            (int16_t)16384


uint_fast8_t sensor_init(struct bmi160_dev*);
void sensor_init_gpio(void);

int8_t sensor_i2c_read(uint8_t, uint8_t, uint8_t*, uint16_t);
int8_t sensor_i2c_write(uint8_t, uint8_t, uint8_t*, uint16_t);
void sensor_delay_ms(uint32_t);

#endif /* SENSOR_H_ */
