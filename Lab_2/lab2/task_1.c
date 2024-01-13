/*
 * task_1.c
 *
 *  Created on: 17 Oct 2020
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
 * Author:  Roman Trueb
 */

#include "lab2.h"                              // Lab specific defines/declarations

/* Global variables */
volatile bool button1Flag;                     // Flag to indicate that button S1 has been pressed
volatile bool button2Flag;                     // Flag to indicate that button S2 has been pressed
volatile uint32_t count1 = 0;                  // Variable to count interrupts produced by S1
volatile uint32_t count2 = 0;                  // Variable to count interrupts produced by S2

void task1(void)
{
  /* Setup UART */
  uart_init(UART_BAUDRATE);

  //m1_1s//
  //// Placeholder 1 (Task 1.1) //////////////////////////////////////////////
  /* Configuring P2.1 (LED2 green) and P2.2 (LED2 blue) as output */
  GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);
  GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);
  ////////////////////////////////////////////////////////////////////////////

  //// Placeholder 2 (Task 1.1) //////////////////////////////////////////////
  /* Configuring P1.1 (Button S1) and P1.4 (Button S2) as an input */
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1);
  GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4);
  ////////////////////////////////////////////////////////////////////////////

  //// Placeholder 3 (Task 1.1) //////////////////////////////////////////////
  /* Configure interrupts for buttons S1 and S2 */
  // Configure P1.1
  GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION);
  GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
  // Configure P1.4
  GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN4, GPIO_LOW_TO_HIGH_TRANSITION);
  GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
  ////////////////////////////////////////////////////////////////////////////

  //// Placeholder 4 (Task 1.1) //////////////////////////////////////////////
  /* Enable interrupts for both GPIO pins */
  GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);
  GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN4);
  /* Enable interrupts on Port 1 */
  Interrupt_enableInterrupt(INT_PORT1);
  /* Enable interrupts globally */
  Interrupt_enableMaster();
  ////////////////////////////////////////////////////////////////////////////

  //// Placeholder 5 (Task 1.1) //////////////////////////////////////////////
  /* Initially turn LEDs off */
  GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);
  GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);
  ////////////////////////////////////////////////////////////////////////////
  //m1_1e//

  /* Code that is executed when processor is not busy with handling an interrupt */
  while (1)
  {
    // Sleep while waiting for an interrupt
    PCM_gotoLPM3();
    // After interrupt occurred
    if (button1Flag)
    {
      /* Increase and send counter for button S1 via UART */
      count1 += 1;
      uart_println("S1: %d", count1);
      button1Flag = false;
    }
    if (button2Flag)
    {
      /* Increase and send counter for button S2 via UART */
      count2 += 1;
      uart_println("S2: %d", count2);
      button2Flag = false;
    }
  }
}

/*
 * PORT 1 interrupt service routine
 */
//m1_2s//
void PORT1_IRQHandler(void)
{
  uint32_t status;

  do {
    /* Get the content of the interrupt status register of Port 1 */
    status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);

    if (status & GPIO_PIN1)
    {
      /* Toggle green LED of LED2 */
      GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1);
      /* Set the flag to inform the while loop that the button has been pressed */
      button1Flag = true;
      GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    }
    else if (status & GPIO_PIN4)
    {
      /* Toggle blue LED of LED2 */
      GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN2);
      /* Set the flag to inform the while loop that the button has been pressed */
      button2Flag = true;
      GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN4);
    }
  } while(status);
}
//m1_2e//

