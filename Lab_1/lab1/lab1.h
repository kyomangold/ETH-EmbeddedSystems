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
 * Author:  Philipp Miedl
 */

#ifndef LAB1_H_
#define LAB1_H_


#define UART_INTERFACE  EUSCI_A0_BASE

#define WAIT_INIT_MULTI ((uint32_t)(15))                 // Multiplier for longer busy wait
#define WAITCYCLES_BASE ((uint32_t)(10000))              // Base value for the wait cycles calculation
#define WAIT_CYCLES     ((uint32_t)(WAITCYCLES_BASE))    // Constant define for the initial number of waiting cycles

extern uint32_t g_waitcycles;                            // Declaration of the global variable defining the number of cycles of the busy loop


/**
 * @brief Embedded Systems (ES) Lab1 Task 1.1
 * 
 * Task 1.1: Flashing an existing application.
 */
void task_1_1(void);

/**
 * @brief Embedded Systems (ES) Lab1 Task 1.2
 * 
 * Task 1.2: Using library functions instead of hard-coded register access
 */
void task_1_2(void);


/**
 * @brief Embedded Systems (ES) Lab1 Task 1.3
 * 
 * Task 1.3: Adding a blinking LED
 */
void task_1_3(void);

/**
 * @brief Embedded Systems (ES) Lab1 Task 2
 * 
 * Task 2: GPIO Input Polling
 */
void task_2_2(void);
void task_2_3(void);

/**
 * @brief Embedded Systems (ES) Lab1 Task 3
 * 
 * Task 3: Simple UART output
 */
void task_3(void);

/**
 * @brief Embedded Systems (ES) Lab1
 * 
 * Task X: Simple UART input from PC
 */
void task_X(void);


#ifdef __DRIVERLIB__H_

/**
 * UART Configuration Parameter. Insert the configuration parameters to
 * make the eUSCI A UART module to operate with the baud rate specified
 * in the handout. These values can be calculated using the online
 * calculator that TI provides at:
 * http://processors.wiki.ti.com/index.php/USCI_UART_Baud_Rate_Gen_Mode_Selection
 */
//----------------uart0----------------
static const eUSCI_UART_Config uart_config  =
{
  EUSCI_A_UART_CLOCKSOURCE_SMCLK,                 // SMCLK Clock Source Selected
  39,                                             // Baudrate Divider BRDIV  = 39
  1,                                              // Clock Compensation Factor UCxBRF =  1
  0,                                              // Clock Compensation Factor UCxBRS =  0
  EUSCI_A_UART_NO_PARITY,                         // No Parity
  EUSCI_A_UART_LSB_FIRST,                         // LSB First
  EUSCI_A_UART_ONE_STOP_BIT,                      // One stop bit
  EUSCI_A_UART_MODE,                              // UART mode
  EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION   // Oversampling Mode
};

//----------------uart1----------------
static const char str_s1[] = "S0 is pressed.";
static const char str_s2[] = "S1 is pressed.";

/**
 * @brief Inline function to configure the
 * UART as needed in this lab session
 */
inline void lab1_configureUART(
     const eUSCI_UART_Config *config
   )
{
 // Selecting P1.2 and P1.3 in UART
 // mode
GPIO_setAsPeripheralModuleFunctionOutputPin
            (
              GPIO_PORT_P1,
              GPIO_PIN2 | GPIO_PIN3,
              GPIO_PRIMARY_MODULE_FUNCTION
            );
 // Configuring UART Module
 UART_initModule(UART_INTERFACE, config);
 // Enable UART module
 UART_enableModule(UART_INTERFACE);
}
//----------------inline1----------------
/**
 * @brief Macro to configure the UART as
 * needed in this lab session
 */
//----------------macro0----------------
#define MACRO_lab1_configureUART(          \
      config                               \
    )                                      \
{                                          \
  /* Selecting P1.2 and P1.3 in UART    */ \
  /* mode                               */ \
GPIO_setAsPeripheralModuleFunctionOutputPin\
            (                              \
              GPIO_PORT_P1,                \
              GPIO_PIN2 | GPIO_PIN3,       \
              GPIO_PRIMARY_MODULE_FUNCTION \
            );                             \
  /* Configuring UART Module            */ \
  UART_initModule(EUSCI_A0_BASE, config);  \
  /* Enable UART module                    \
  UART_enableModule(EUSCI_A0_BASE);     */ \
}
//----------------macro1----------------

/**
 * @brief Send string via UART peripheral
 * @param str Pointer to the message to send
 * @param ... printf-style parameters
 */
inline void uart_println(const char* str, ...)
{
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
}

#endif /* __DRIVERLIB__H_ */

#endif /* LAB1_H_ */
