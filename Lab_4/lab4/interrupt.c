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

#include <stdint.h>

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include "app.h"
#include "gpio.h"
#include "interrupt.h"


/**
 * GPIO port 1 interrupt handler routine.
 */
void PORT1_IRQHandler(void)
{
  // Get interrupt flags
  uint_fast16_t status = GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);

  /**
   * Task 3.2
   *
   * @todo Check interrupt flags and take required actions.
   * - If left button S1 is pressed: request using the next sensor reading
   *   as sensor offset value by calling vAppStoreCalibration().
   * - If right button S2 is pressed: reset any stored calibration offset
   *   to the default zero value by calling vAppResetCalibration().
   *
   * Hint:
   * The button port and pins definitions for S1 and S2 are available from
   * gpio.h. The above mentioned functions are declared in the app.h file.
   * Use the definitions of these headers, they are already included at the
   * beginning of this file.
   */

  //m3_2s//
  // MSP432F401R Launchpad button S1 (left)
  if (status & BUTTON_S1_PIN)
  {
    vAppStoreCalibration();
    // Clear flag of processed interrupt
    GPIO_clearInterruptFlag(BUTTON_S1_PORT, BUTTON_S1_PIN);
  }
  // MSP432F401R Launchpad button S2 (right)
  if (status & BUTTON_S2_PIN)
  {
    vAppResetCalibration();
    // Clear flag of processed interrupt
    GPIO_clearInterruptFlag(BUTTON_S2_PORT, BUTTON_S2_PIN);
  }
  //m3_2e//
}