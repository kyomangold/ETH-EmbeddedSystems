/*
 * task 1.2, 1.3, 2.4
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

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "gpio.h"
#include "uart.h"

#include "app.h"

/**
 * UART baudrate configuration.
 */
const uint32_t uart_baudrate = 115200;

/**
 * FreeRTOS task handle for the Sensor task.
 */
TaskHandle_t xSensorTaskHandle = NULL;

/**
 * FreeRTOS task handle for the Processing task.
 */
TaskHandle_t xProcessingTaskHandle = NULL;

/**
 * FreeRTOS task handle for UART Print task.
 */
TaskHandle_t xPrintTaskHandle = NULL;

/**
 * FreeRTOS task handle for the LED task.
 */
TaskHandle_t xLedTaskHandle = NULL;

/**
 * FreeRTOS queue handle for sensor data queue.
 * This queue is used to communicate sensor data from the Sensor to the
 * Processing task. Task handle for Sensor task.
 */
QueueHandle_t xSensorQueueHandle = NULL;

/**
 * FreeRTOS queue handle for print data queue.
 *
 * This queue is used to communicate processed attitude data from the
 * Processing to the UART Print task.
 */
QueueHandle_t xPrintQueueHandle = NULL;

/**
 * FreeRTOS queue handle for LED data queue.
 *
 * This queue is used to communicate processed attitude data from the
 * Processing to the LED indicator task.
 */
QueueHandle_t xLedQueueHandle = NULL;

/* ------------------------------------------------------------------------- */

/**
 * Main function initializes basic hardware, setups tasks and queues and
 * finally starts the FreeRTOS scheduler.
 *
 * @return Program exit value.
 */
int main(void)
{
  // Init hardware
  gpio_init();
  uart_init(uart_baudrate);

  /**
   * Tasks 1.2
   *
   * @todo Create the three queues (Sensor, Print and LED queue) used in the
   * application:
   * - The Sensor queue holds elements of type `sensor_data_t` with a length
   *   of up to up to `appSENSOR_QUEUE_LENGTH`.
   * - The Print queue holds elements of type `attitude_data_t` with a length
   *   of up to up to `appPRINT_QUEUE_LENGTH`.
   * - The LED queue holds elements of type `attitude_data_t` with a length
   *   of up to up to `appLED_QUEUE_LENGTH`.
   *
   * Hint:
   * Use predefined queue handles defined at the beginning of this file. The
   * queue related data structures and defines are declared in app.h (which
   * is already included at the top of this file). Check the return value of
   * the queue create functions for errors.
   */


  //m1_2s//
  BaseType_t result = pdPASS;

  // Create Sensor data queue
  xSensorQueueHandle = xQueueCreate(appSENSOR_QUEUE_LENGTH, sizeof(sensor_data_t));
  if (xSensorQueueHandle == NULL)
  {
    uart_println("Error creating Sensor data queue.");
    return 0;
  }

  // Create Print data queue
  xPrintQueueHandle = xQueueCreate(appPRINT_QUEUE_LENGTH, sizeof(attitude_data_t));
  if (xPrintQueueHandle == NULL)
  {
    uart_println("Error creating Print data queue.");
    return 0;
  }

  // Create LED data queue
  xLedQueueHandle = xQueueCreate(appLED_QUEUE_LENGTH, sizeof(attitude_data_t));
  if (xPrintQueueHandle == NULL)
  {
    uart_println("Error creating LED data queue.");
    return 0;
  }
  //m1_2e//


  /**
   * Tasks 1.3
   *
   * @todo Create the three initial tasks of the application (Sensor, Print
   * and LED task):
   * - The Sensor task is implement in the function `prvSensorTask`, takes an
   *   argument of type `sensor_task_param_t` and runs with priority
   *   `appSENSOR_TASK_PRIORITY`.
   * - The Print task is implement in the function `prvPrintTask`, takes an
   *   argument of type `print_task_param_t` and runs with priority
   *   `appPRINT_TASK_PRIORITY`.
   * - The LED task is implement in the function `prvLedTask`, takes an
   *   argument of type `led_task_param_t` and runs with priority
   *   `appLED_TASK_PRIORITY`.
   *
   * Hint:
   * Use predefined task handles defined at the beginning of this file. The
   * task related data structures and defines are declared in app.h (which is
   * already included at the top of this file). Check the return value of the
   * task create functions for errors.
   */


  //m1_3s//
  // Create the task used to read the motion sensors
  sensor_task_param_t sensor_task_param = {
    .outDataQueue = xSensorQueueHandle,
  };
  result = xTaskCreate(prvSensorTask,
                       "Sensor",
                       appDEFAULT_TASK_STACK_SIZE,
                       &sensor_task_param,
                       appSENSOR_TASK_PRIORITY,
                       &xSensorTaskHandle);
  if (result != pdPASS)
  {
    uart_println("Error creating Sensor task.");
    return 0;
  }

  // Create the task used to send data via UART
  print_task_param_t print_task_param = {
    .inDataQueue = xPrintQueueHandle,
  };
  result = xTaskCreate(prvPrintTask,
                       "Print",
                       appPRINT_TASK_STACK_SIZE,
                       &print_task_param,
                       appPRINT_TASK_PRIORITY,
                       &xPrintTaskHandle);
  if (result != pdPASS)
  {
    uart_println("Error creating Print task.");
    return 0;
  }

  // Create the task used to indicate the current angle using the LED
  led_task_param_t led_task_param = {
    .inDataQueue = xLedQueueHandle,
  };
  result = xTaskCreate(prvLedTask,
                       "LED",
                       appDEFAULT_TASK_STACK_SIZE,
                       &led_task_param,
                       appLED_TASK_PRIORITY,
                       &xLedTaskHandle);
  if (result != pdPASS)
  {
    uart_println("Error creating LED task.");
    return 0;
  }
  //m1_3e//


  /**
   * Task 2.4
   *
   * @todo Create the processing task that you implemented from scratch. Set
   * the task priority to `appPROCESSING_TASK_PRIORITY` and decide on your
   * own on a reasonable stack size for your processing task.
   *
   * Hint:
   * Again use the predefined task handle defined at the beginning of this
   * file. The required data structures and defines are declared in app.h.
   */


  //m2_4s//
  // Create the task used to process sensor data
  processing_task_param_t processing_task_param = {
    .inDataQueue = xSensorQueueHandle,
    .outLedQueue = xLedQueueHandle,
    .outPrintQueue = xPrintQueueHandle,
  };
  result = xTaskCreate(prvProcessingTask,
                       "Processing",
                       appDEFAULT_TASK_STACK_SIZE,
                       &processing_task_param,
                       appPROCESSING_TASK_PRIORITY,
                       &xProcessingTaskHandle);
  if (result != pdPASS)
  {
    uart_println("Error creating Processing task.");
    return 0;
  }
  //m2_4e//


  // Print end of task and queue initialization
  uart_println("Queues and tasks initialized.");

  // Enable interrupts globally
  Interrupt_enableMaster();

  // Start the tasks and timer running
  vTaskStartScheduler();

  /*
   * If all is well, the scheduler will now be running, and the following
   * line will never be reached.  If the following line does execute, then
   * there was insufficient FreeRTOS heap memory available for the idle and/or
   * timer tasks to be created.  See the memory management section on the
   * FreeRTOS web site for more details.
   */
  return 0;
}

/* -------------------------------------------------------------------------
 * Default FreeRTOS hooks
 * ------------------------------------------------------------------------- */

void vApplicationMallocFailedHook(void)
{
  /*
   * vApplicationMallocFailedHook() will only be called if
   * configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h. It is a
   * hook function that will get called if a call to pvPortMalloc() fails.
   * pvPortMalloc() is called internally by the kernel whenever a task, queue,
   * timer or semaphore is created.  It is also called by various parts of the
   * demo application.  If heap_1.c or heap_2.c are used, then the size of the
   * heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
   * FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
   * to query the size of free heap space that remains (although it does not
   * provide information on how the remaining heap might be fragmented).
   */
  taskDISABLE_INTERRUPTS();
  for (;;);
}
/* ------------------------------------------------------------------------- */


void vApplicationIdleHook(void)
{
  /*
   * vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   * to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the
   * idle task. It is essential that code added to this hook function never
   * attempts to block in any way (for example, call xQueueReceive() with a
   * block time specified, or call vTaskDelay()).  If the application makes
   * use of the vTaskDelete() API function (as this demo application does)
   * then it is also important that vApplicationIdleHook() is permitted to
   * return to its calling function, because it is the responsibility of the up
   * idle task to clean memory allocated by the kernel to any task that has
   * since been deleted.
   */
}
/* ------------------------------------------------------------------------- */


void vApplicationStackOverflowHook(TaskHandle_t pxTask, char* pcTaskName)
{
  (void)pcTaskName;
  (void)pxTask;

  /*
   * Run time stack overflow checking is performed if
   * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2. This hook function
   * is called if a stack overflow is detected.
   */
  taskDISABLE_INTERRUPTS();
  for (;;);
}
/* ------------------------------------------------------------------------- */


void* malloc(size_t xSize)
{
  /*
   * There should not be a heap defined, so trap any attempts to call
   * malloc.
   */
  Interrupt_disableMaster();
  for (;;);
}
/* ------------------------------------------------------------------------- */
