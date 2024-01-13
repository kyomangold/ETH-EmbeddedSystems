/*
 * task 3.1
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

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "gpio.h"


void gpio_init(void)
{
  // On-board LEDs
  GPIO_setAsOutputPin(LED1_PORT, LED1_PIN);
  GPIO_setAsOutputPin(LED2_RED_PORT, LED2_RED_PIN);
  GPIO_setAsOutputPin(LED2_GREEN_PORT, LED2_GREEN_PIN);
  GPIO_setAsOutputPin(LED2_BLUE_PORT, LED2_BLUE_PIN);

  GPIO_setOutputLowOnPin(LED1_PORT, LED1_PIN);
  GPIO_setOutputLowOnPin(LED2_BLUE_PORT, LED2_BLUE_PIN);
  GPIO_setOutputLowOnPin(LED2_GREEN_PORT, LED2_GREEN_PIN);
  GPIO_setOutputLowOnPin(LED2_BLUE_PORT, LED2_BLUE_PIN);

  // On-board buttons
  GPIO_setAsInputPinWithPullUpResistor(BUTTON_S1_PORT, BUTTON_S1_PIN);
  GPIO_setAsInputPinWithPullUpResistor(BUTTON_S2_PORT, BUTTON_S2_PIN);

  /**
   * Task 3.1
   *
   * @todo Configure and enable the button press interrupts for S1 and S2.
   * - Check the LaunchPad schematic whether to trigger an interrupts on HIGH
   *   to LOW or LOW to HIGH transition.
   * - Clear the interrupt flags.
   * - Enable the required interrupts.
   *
   * Hint:
   * Do not forget to globally enable the interrupt for the selected GPIO
   * port module at the end, using the `Interrupt_enableInterrupt()`
   * DriverLib function.
   */

  //m3_1s//
  // Interrupt setup
  GPIO_interruptEdgeSelect(BUTTON_S1_PORT, BUTTON_S1_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
  GPIO_clearInterruptFlag(BUTTON_S1_PORT, BUTTON_S1_PIN);
  GPIO_enableInterrupt(BUTTON_S1_PORT, BUTTON_S1_PIN);

  GPIO_interruptEdgeSelect(BUTTON_S2_PORT, BUTTON_S2_PIN, GPIO_HIGH_TO_LOW_TRANSITION);
  GPIO_clearInterruptFlag(BUTTON_S2_PORT, BUTTON_S2_PIN);
  GPIO_enableInterrupt(BUTTON_S2_PORT, BUTTON_S2_PIN);

  Interrupt_enableInterrupt(INT_PORT1);
  //m3_1e//
}
