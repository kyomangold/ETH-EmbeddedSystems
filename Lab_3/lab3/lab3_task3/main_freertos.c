/*
 * task_3.c
 *
 *  Created on: 30 Oct 2020
 *      Author: Kyo
 */

#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"
#include "semphr.h"
#include "queue.h"

/**
 * UART baudrate configuration.
 */
const uint32_t uart_baudrate = 115200;


/***
 * Declaration of functions
 */
void vSquareTaskFunction(void *pvParameters);
void vDecrementTaskFunction(void *pvParameters);

/***
 * A struct which contains a handle for two queues.
 * One queue sends the parameter from the SquareTask to the DecrementTask and
 * one sends it the other way round.
 */
//m3_1_2s//
typedef struct {
    QueueHandle_t sq2decr;
    QueueHandle_t decr2sq;
} qhandles_params_t;
//m3_1_2e//

/***
 * Function for printing to the UART terminal, using a mutex to avoid access conflicts.
 */
void uart_println_mutex(const char* str, ...);

/* The semaphore handle, which is used in uart_println_mutex() */
SemaphoreHandle_t xMutex;

/*-----------------------------------------------------------*/
int main(void)
{
    // Initialize UART
    uart_init(uart_baudrate);

    BaseType_t result = pdPASS;

    xMutex = xSemaphoreCreateMutex();

    /***
     * Create the queues and keep the handle, which is then stored in our
     * parameter struct and assigned as a parameter to the task function.
     */
    //m3_1_1s//
    QueueHandle_t xSq2DecrHandle = xQueueCreate( 1, sizeof(uint32_t));
    QueueHandle_t xDecr2SqHandle = xQueueCreate( 1, sizeof(uint32_t));
    //m3_1_1e//

    //m3_1_3s//
    qhandles_params_t qhandles_params = {
        .sq2decr = xSq2DecrHandle,
        .decr2sq = xDecr2SqHandle,
    };
    void *pvParameters = (void*)&qhandles_params;

    // Create the task used to send data via UART
    uart_println_mutex("Creating SquareTask.");
    result =  xTaskCreate(vSquareTaskFunction, "SquareTask", 2000, pvParameters, 1, NULL);
    if (result != pdPASS) {
        uart_println_mutex("Error creating SquareTask task.");
        return 0;
    }

    uart_println_mutex("Creating DecrementTask.");
    result =  xTaskCreate(vDecrementTaskFunction, "DecrementTask", 2000, pvParameters, 1, NULL);
    if (result != pdPASS) {
        uart_println_mutex("Error creating DecrementTask task.");
        return 0;
    }
    //m3_1_3e//

    // Start the tasks and timer running
    vTaskStartScheduler();

    /* The following line should never be reached.
     * Otherwise there was insufficient FreeRTOS heap
     * memory available for the task to be created.
     */
    return 0;
}
/*-----------------------------------------------------------*/


/**
 * The task initalizes a variable with 4,
 * transmits the value to the DecrementTask and waits for a value to be received
 * from the DecrementTask before squaring it
 * @param pvParameters: qhandles_params_t struct containing the queue handles
 */
//m3_1_4s//
void vSquareTaskFunction(void *pvParameters)
{
    uart_println_mutex("Start square task.");
    // Convert our parameter from (void*) to the qhandles_params_t struct.
    qhandles_params_t handles =  *((qhandles_params_t*)pvParameters);

    uint32_t var=4;
    while (1) {
        // Send the value to the DecrementTask
        xQueueSendToBack(handles.sq2decr,(void*)&var,portMAX_DELAY);
        // Blocking wait until we receive from the DecrementTask. This will overwrite our local variable
        xQueueReceive(handles.decr2sq,(void*)&var,portMAX_DELAY);
        uart_println_mutex("Value: %d - SquareTask",var);
        if(var > 10000)
            vTaskDelete(NULL);
        var = var * var;
    }
}

/**
 * The task initalizes a variable with 0,
 * receives the value from the SquareTask and decrements the value before sending
 * it back to the SquareTask
 * @param pvParameters: qhandles_params_t struct containing the queue handles
 */
void vDecrementTaskFunction(void *pvParameters)
{
    uart_println_mutex("Start decrement task.");
    // Convert our parameter from (void*) to the qhandles_params_t struct.
    qhandles_params_t handles =  *((qhandles_params_t*)pvParameters);

    uint32_t var=0;
    while (1) {
        // Blocking wait until we receive from the SquareTask. This will overwrite our local variable
        xQueueReceive(handles.sq2decr,(void*)&var,portMAX_DELAY);
        uart_println_mutex("Value: %d - DecrementTask",var);
        if(var > 10000)
            vTaskDelete(NULL);
        var = var - 1;
        // Send the value to the SquareTask
        xQueueSendToBack(handles.decr2sq,(void*)&var,portMAX_DELAY);
    }
}
//m3_1_4e//

/***
 * A custom print function, which is extended to use a mutex to avoid access conflicts.
 * @param str Pointer to the message to send
 * @param ... printf-style parameters
 */
void uart_println_mutex(const char* str, ...)
{
    xSemaphoreTake(xMutex, portMAX_DELAY);

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

    xSemaphoreGive(xMutex);
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
