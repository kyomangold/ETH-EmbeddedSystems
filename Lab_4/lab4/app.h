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

#ifndef APP_H_
#define APP_H_

#include <math.h>
#include <stdint.h>

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include <bosch/bmi160/bmi160.h>

/**
 * Pi constant to use for angle calculations.
 */
#define PI 3.14159265358979323846

/**
 * The Sensor task's scheduling priority.
 */
#define appSENSOR_TASK_PRIORITY     ( tskIDLE_PRIORITY + 1 )

/**
 * The Processing task's scheduling priority.
 */
#define appPROCESSING_TASK_PRIORITY ( tskIDLE_PRIORITY + 2 )

/**
 * The Print task's scheduling priority.
 */
#define appPRINT_TASK_PRIORITY      ( tskIDLE_PRIORITY + 4 )

/**
 * The LED task's scheduling priority.
 */
#define appLED_TASK_PRIORITY        ( tskIDLE_PRIORITY + 3 )

/**
 * Default stack size for smaller tasks.
 */
#define appDEFAULT_TASK_STACK_SIZE  400

/**
 * Stack size for the Print task.
 */
#define appPRINT_TASK_STACK_SIZE    800

/**
 * The Sensor data queue length.
 */
#define appSENSOR_QUEUE_LENGTH      1

/**
 * The Print data queue length.
 */
#define appPRINT_QUEUE_LENGTH       1

/**
 * The LED data queue length.
 */
#define appLED_QUEUE_LENGTH         1

/**
 * The angle threshold in degrees at which a warning is shown (red LED).
 */
#define appWARNING_ANGLE_DEG        30

/**
 * The significant angle change threshold in degrees to forward the calculation
 * result to the output tasks (i.e. print and LED tasks).
 */
#define appANGLE_SIGNIFICANT_CHANGE 5

/**
 * Sensor data structure.
 */
typedef struct _sensor_data_t
{
  uint32_t time;
  int16_t acc_x;
  int16_t acc_y;
  int16_t acc_z;
  int16_t gyro_x;
  int16_t gyro_y;
  int16_t gyro_z;
} sensor_data_t;

/**
 * Attitude data structure.
 */
typedef struct _attitude_data_t
{
  uint32_t time;
  float theta_x;
  float theta_y;
} attitude_data_t;

/**
 * Sensor task parameter struct.
 */
typedef struct _sensor_task_param_t
{
  QueueHandle_t outDataQueue;
} sensor_task_param_t;

/**
 * Print task parameter struct.
 */
typedef struct _print_task_param_t
{
  QueueHandle_t inDataQueue;
} print_task_param_t;

/**
 * Processing task parameter struct.
 */
typedef struct _processing_task_param_t
{
  QueueHandle_t inDataQueue;
  QueueHandle_t outPrintQueue;
  QueueHandle_t outLedQueue;
} processing_task_param_t;

/**
 * LED task parameter struct.
 */
typedef struct _led_task_param_t
{
  QueueHandle_t inDataQueue;
} led_task_param_t;

extern volatile struct bmi160_sensor_data calibration_acc;
extern volatile uint_fast8_t calibration_flag;

void prvSensorTask(void *pvParameters);
void prvProcessingTask(void *pvParameters);
void prvPrintTask(void *pvParameters);
void prvLedTask(void *pvParameters);

/**
 * Set flag to store next measurement as acceleration calibration offset.
 */
inline void vAppStoreCalibration(void)
{
  calibration_flag = 1;
}

/**
 * Reset the acceleration calibration offset.
 */
inline void vAppResetCalibration(void)
{
  calibration_acc.x = 0;
  calibration_acc.y = 0;
  calibration_acc.z = 0;
}

#endif /* APP_H_ */
