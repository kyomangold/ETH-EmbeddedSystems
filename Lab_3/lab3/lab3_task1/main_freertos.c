/*
 * task_1.c
 *
 *  Created on: 30 Oct 2020
 *      Author: Kyo
 */


#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"

#include "uart.h"


/**
 * UART baudrate configuration.
 */
const uint32_t uart_baudrate = 115200;


/***
 * Declaration of functions
 */
void vTaskAFunction(void *pvParameters);
void vTaskBFunction(void *pvParameters);

void vSimpleDelay();

/***
 * Function for printing to the UART terminal. This function will be modified in Task 2.1
 * Without modification it works exactly like uart_println()
 */
void uart_println_mutex(const char* str, ...);

//m1_3_1s//
/* The counter which is used to check when a task should be deleted */
int counter = 0;

/* The handle to TaskA, which can be used to delete TaskA */
TaskHandle_t xTaskAHandle = NULL;
//m1_3_1e//

/*-----------------------------------------------------------*/
int main(void)
{
    // Initialize UART
    uart_init(uart_baudrate);

    BaseType_t result = pdPASS;

    //m1_1_2s//
    // The first task will be created and the string to be printed will be passed as a parameter
    uart_println("Creating TaskA.");
    char *pcParameters1 = "TaskA is running and running.";
    result =  xTaskCreate(vTaskAFunction, "TaskA", 1000, (void*)pcParameters1, 1, &xTaskAHandle);
    if (result != pdPASS) {
        uart_println("Error creating TaskA task.");
        return 0;
    }
    //m1_1_2e//

    /***
     * The second task will be created and the string to be printed will be passed as a parameter.
     * Note: For Task 1.1. we could also use the same task function (i.e. vTask1Function) and only
     * change the parameter.
     */
    //m1_2s//
    uart_println("Creating TaskB.");
    char *pcParameters2 = "TaskB is running and running.";
    result =  xTaskCreate(vTaskBFunction, "TaskB", 1000, (void*)pcParameters2, 1, NULL);
    if (result != pdPASS) {
        uart_println("Error creating TaskB task.");
        return 0;
    }
    //m1_2e//

    // Start the tasks
    vTaskStartScheduler();

    /* The following line should never be reached.
     * Otherwise there was insufficient FreeRTOS heap
     * memory available for the task to be created.
     */
    return 0;
}
/*-----------------------------------------------------------*/


/**
 * TaskA
 * Prints its name repeatedly
 * @param pvParameters String with the task name
 */
//m1_1_1s//
void vTaskAFunction(void *pvParameters)
{
    char *message = (char*)pvParameters;
    uart_println("Start TaskA.");

    while (1) {
        uart_println(message);
        vSimpleDelay();
    }
}
//m1_1_1e//

/**
 * TaskB
 * Prints its name repeatedly. Increments a global counter
 * and deletes TaskA after 10 iterations and TaskB after 20.
 * @param pvParameters String with the task name
 */
//m1_3_3s//
void vTaskBFunction(void *pvParameters)
{
    char *message = (char*)pvParameters;
    uart_println("Start TaskB.");

    while (1) {
        uart_println("%d: ",counter);
        uart_println(message);
        if(counter == 10) {
            vTaskDelete(xTaskAHandle);
        }
        else if(counter ==  20) {
            vTaskDelete(NULL);
        }
        counter++;
        vSimpleDelay();
    }
}
//m1_3_3e//

/***
 * Wait for a given amount of time
 */
void vSimpleDelay()
{
    for(uint32_t i=0; i < 100000;i++)
    {
    }
}

/***
 * A custom print function, which shall be extended in Task 2.1.
 * @param str Pointer to the message to send
 * @param ... printf-style parameters
 */
void uart_println_mutex(const char* str, ...)
{
    // TODO: You can insert your code here

    // ======= DO NOT EDIT BELOW THIS LINE =======
    static char print_buffer[256];

    va_list lst;
    va_start(lst, str);
    vsnprintf(print_buffer, 256, str, lst);
    str = print_buffer;
    while (*str)
    {
        while (!(EUSCI_A_CMSIS(UART_INTERFACE)->IFG & EUSCI_A_IFG_TXIFG));
        EUSCI_A_CMSIS(UART_INTERFACE)->TXBUF = *str;
        str++;
    }
    while (!(EUSCI_A_CMSIS(UART_INTERFACE)->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A_CMSIS(UART_INTERFACE)->TXBUF = '\r';
    while (!(EUSCI_A_CMSIS(UART_INTERFACE)->IFG & EUSCI_A_IFG_TXIFG));
    EUSCI_A_CMSIS(UART_INTERFACE)->TXBUF = '\n';
    // ======= DO NOT EDIT ABOVE THIS LINE =======

    // TODO: You can insert your code here
}


/* -------------------------------------------------------------------------
 * Default FreeRTOS hooks
 * ------------------------------------------------------------------------- */

void vApplicationMallocFailedHook( void )
{
    /* vApplicationMallocFailedHook() will only be called if
    configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
    function that will get called if a call to pvPortMalloc() fails.
    pvPortMalloc() is called internally by the kernel whenever a task, queue,
    timer or semaphore is created.  It is also called by various parts of the
    demo application.  If heap_1.c or heap_2.c are used, then the size of the
    heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
    FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
    to query the size of free heap space that remains (although it does not
    provide information on how the remaining heap might be fragmented). */
    taskDISABLE_INTERRUPTS();
    for( ;; );
}
/* ------------------------------------------------------------------------- */

void vApplicationIdleHook( void )
{
    /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
    to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
    task.  It is essential that code added to this hook function never attempts
    to block in any way (for example, call xQueueReceive() with a block time
    specified, or call vTaskDelay()).  If the application makes use of the
    vTaskDelete() API function (as this demo application does) then it is also
    important that vApplicationIdleHook() is permitted to return to its calling
    function, because it is the responsibility of the idle task to clean up
    memory allocated by the kernel to any task that has since been deleted. */
}
/* ------------------------------------------------------------------------- */

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
    configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();
    for( ;; );
}
/* ------------------------------------------------------------------------- */

void *malloc( size_t xSize )
{
    /* There should not be a heap defined, so trap any attempts to call
    malloc. */
    Interrupt_disableMaster();
    for( ;; );
}
/* ------------------------------------------------------------------------- */


/*-----------------------------------------------------------*/

void vPreSleepProcessing(uint32_t ulExpectedIdleTime)
{

}
/*-----------------------------------------------------------*/

void vApplicationTickHook(void)
{
    /* This function will be called by each tick interrupt if
     configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
     added here, but the tick hook is called from an interrupt context, so
     code must not attempt to block, and only the interrupt safe FreeRTOS API
     functions can be used (those that end in FromISR()). */

    /* The full demo includes a software timer demo/test that requires
     prodding periodically from the tick interrupt. */
    //vTimerPeriodicISRTests();
    /* Call the periodic event group from ISR demo. */
    //vPeriodicEventGroupsProcessing();
    /* Use task notifications from an interrupt. */
    //xNotifyTaskFromISR();
    /* Use mutexes from interrupts. */
    //vInterruptSemaphorePeriodicTest();
}
/*-----------------------------------------------------------*/
