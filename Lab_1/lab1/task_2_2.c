/*
 * task_2_2.c
 *
 *  Created on: 03 Oct 2020
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
 * Author:  Philipp Miedl
 */

#include <msp.h>                                // Platform specific header (HW definitions)
#include <stdint.h>                             // Standard Integer - data type definitions
#include <stdio.h>                              // Standard In-/Output definitions/functions

#include "ESLab1driverLib/driverlib.h"          // TI Platform library (subset)
#include "lab1.h"                               // Lab specific defines/declarations

void task_2_2(void)
{
  WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;         // Stopping the Watchdog Timer

  uint32_t count = 0;                                 // Simple counter variable
  GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);       // Set P1.0 (LED1) to output direction
  GPIO_setAsOutputPin(GPIO_PORT_P2,                   // Set P2.0, P2.1 & P2.2 (LED2) ...
                      GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2
                     );                               // ...to output direction

  GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0);   // Make sure LED1 starts in high state
  GPIO_setOutputLowOnPin(GPIO_PORT_P2,                // Make sure LED2 start...
                         GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2
                        );                            // ...in low state

  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1,  // Set P1.1 & P1.4 (buttons S1 and...
                                       GPIO_PIN1 | GPIO_PIN4
                                      );              // ... S2) as inputs with pull-up
  while(1)
  {
    GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);  // Toggle LED1
    GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN0);  // Toggle LED2 Red

    // ---------------------------------------------------------------------------------- //
    //                                    Placeholder 1                                   //
    if(GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN1) == ((uint8_t)0x00))  // Poll button S1
    {
      GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1);    // Set LED2 Green to high
    }
    else
    {
      GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);     // Set LED2 Green to low
    }
    if(GPIO_getInputPinValue(GPIO_PORT_P1,GPIO_PIN4) == ((uint8_t)0x00))  // Poll button S2
    {
      GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2);    // Set LED2 Blue to high
    }
    else
    {
      GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);     // Set LED2 Blue to low
    }
    // ---------------------------------------------------------------------------------- //
    for(count = 0; count < g_waitcycles; count++)
    {
      // -------------------------------------------------------------------------------- //
      //                                  Placeholder 2                                   //
      // -------------------------------------------------------------------------------- //
    }
  }
}
