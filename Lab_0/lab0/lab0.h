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
 * Author:  Romain Jacob
 */

#ifndef LAB0_H_
#define LAB0_H_


#include <msp.h>                           // Platform specific header (HW definitions)
#include <stdint.h>                        // Standard Integer Library - datatype definitions
#include <stdio.h>                         // Standard In-/Output definitions/functions


inline void toggle(uint8_t LEDpins)
{
  P1OUT ^= (LEDpins & 0x01);
  P2OUT ^= (LEDpins & 0x0e) >> 1;
}


inline void uart_println(const char* str, ...)
{
  static char print_buffer[256];
  va_list lst;
  va_start(lst, str);
  vsnprintf(print_buffer, 256, str, lst);
  str = print_buffer;
  while (*str)
  {
    while (!(UCA0IFG & EUSCI_A_IFG_TXIFG));
    UCA0TXBUF = *str;
    str++;
  }
  while (!(UCA0IFG & EUSCI_A_IFG_TXIFG));
  UCA0TXBUF = '\r';
  while (!(UCA0IFG & EUSCI_A_IFG_TXIFG));
  UCA0TXBUF = '\n';
}


/**
 * @brief Embedded Systems (ES) Lab1
 *
 * Base scaffold for the Lab1 in Embedded Systems (ES). The goal of this
 * Lab is that the students get to know the platform MSP432P401R LaunchPad
 * Development Kit. This file implements the following part:
 * Task 2: GPIO Input Polling
 */
void task_2(void);

/**
 * @brief Embedded Systems (ES) Lab1
 *
 * Base scaffold for the Lab1 in Embedded Systems (ES). The goal of this
 * Lab is that the students get to know the platform MSP432P401R LaunchPad
 * Development Kit. This file implements the following part:
 * Task 3: Simple UART output
 */
void task_3(void);

/**
 * @brief Embedded Systems (ES) Lab1
 *
 * Base scaffold for the Lab1 in Embedded Systems (ES). The goal of this
 * Lab is that the students get to know the platform MSP432P401R LaunchPad
 * Development Kit. This file implements the following part:
 * Task X: Simple UART input from PC
 */
void task_4(void);

/**
 * @brief Embedded Systems (ES) Lab1
 *
 * Base scaffold for the Lab1 in Embedded Systems (ES). The goal of this
 * Lab is that the students get to know the platform MSP432P401R LaunchPad
 * Development Kit. This file implements the following part:
 * Task X: Simple UART input from PC
 */
void task_5(void);


#endif /* LAB0_H_ */
