/*
 * task_2.c
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

#include "lab2.h"                       // Lab specific defines/declarations

/* Application defines  */
#define TA0_CCR0_BREATH           499
#define PWM_CCR0                  127
#define COMPARE_VAL_INC_STEP        1

/* Port mapper configuration register */
const uint8_t port_mapping[] =
{
  PMAP_TA1CCR1A, PMAP_NONE, PMAP_NONE, PMAP_NONE, PMAP_NONE, PMAP_NONE, PMAP_NONE, PMAP_NONE
};

void task2(void)
{
//m2_1s//
  /* Timer_A0 up mode configuration parameters */
  const Timer_A_UpModeConfig upConfigA0 =
  {
    TIMER_A_CLOCKSOURCE_SMCLK,           // SMCLK clock source (3MHz)
    TIMER_A_CLOCKSOURCE_DIVIDER_64,      // SMCLK/64 = 46.875kHz
    TA0_CCR0,                            // Value in CCR0, NOTE: limited to 16 bit!
    TIMER_A_TAIE_INTERRUPT_DISABLE,      // Disable timer roll-over interrupt
    TIMER_A_CCIE_CCR0_INTERRUPT_ENABLE,  // Enable CCR0 interrupt
    TIMER_A_DO_CLEAR                     // Clear value in the timer counter register
  };

  //// Placeholder 1 (Task 2.1) //////////////////////////////////////////////
  /* Configuring P1.0 (LED1) as output */
  GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0);
  GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0);
  ////////////////////////////////////////////////////////////////////////////

  //// Placeholder 2 (Task 2.1) //////////////////////////////////////////////
  /* Configuring Timer_A0 for Up mode */
  Timer_A_configureUpMode(TIMER_A0_BASE, &upConfigA0);
  ////////////////////////////////////////////////////////////////////////////

  //// Placeholder 3 (Task 2.1) /////////////////////////////////////////////
  /* Clear interrupt */
  Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,
                                       TIMER_A_CAPTURECOMPARE_REGISTER_0);
  /* Enable interrupt */
  Interrupt_enableInterrupt(INT_TA0_0);
  /* Enable interrupts globally */
  Interrupt_enableMaster();
  /* Start the Timer_A0 */
  Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
  ////////////////////////////////////////////////////////////////////////////
//m2_1e//

  /* Remapping TA1CCR1 output pin to P2.0 (red LED of LED2) */
  PMAP_configurePorts((const uint8_t *) port_mapping,
                      PMAP_P2MAP,
                      1,
                      PMAP_DISABLE_RECONFIGURATION);

  /* Set pin P2.0 as output pin */
  GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P2,
                                              GPIO_PIN0,
                                              GPIO_PRIMARY_MODULE_FUNCTION);

//m2_5s//
  /* Timer_A1 up mode configuration parameters */
  const Timer_A_UpModeConfig upConfigA1 =
  {
    TIMER_A_CLOCKSOURCE_SMCLK,              // SMCLK clock source
    TIMER_A_CLOCKSOURCE_DIVIDER_1,          // SMCLK/1 = 3MHz
    PWM_CCR0,                               // Value in CCR0
    TIMER_A_TAIE_INTERRUPT_DISABLE,         // Disable Timer interrupt
    TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE,    // Disable CCR0 interrupt
    TIMER_A_DO_CLEAR                        // Clear value
  };

  /* Timer_A1 compare configuration parameters */
  Timer_A_CompareModeConfig compareConfigA1 =
  {
    TIMER_A_CAPTURECOMPARE_REGISTER_1,          // Use CCR1 as compare register
    TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
    TIMER_A_OUTPUTMODE_TOGGLE_RESET,            // Toggle-reset output mode
    125                                         // Compare value (CCR1)
  };

  //// Placeholder 4 (Task 2.2) //////////////////////////////////////////////
  /* Configure the compare functionality of Timer_A1 */
  Timer_A_initCompare(TIMER_A1_BASE, &compareConfigA1);
  /* Configure Timer_A1 for Up Mode */
  Timer_A_configureUpMode(TIMER_A1_BASE, &upConfigA1);
  /* Start counter of Timer_A1 */
  Timer_A_startCounter(TIMER_A1_BASE, TIMER_A_UP_MODE);
  ////////////////////////////////////////////////////////////////////////////
//m2_5e//

  /* Sleeping when not in use */
  while (1)
  {
    PCM_gotoLPM3();
  }
}

//m2_2s//
void TA0_0_IRQHandler(void)
{
//m2_2e//
//m2_3s//
  //// Placeholder 5 (Task 2.1) //////////////////////////////////////////////
  /* Toggle LED1 */
  GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0);
  ////////////////////////////////////////////////////////////////////////////
//m2_3e//

//m2_6s//
  /* Definitions for Task 2.3 */
  static int16_t pwmCompareVal = PWM_CCR0;    // Current duty cycle value
  static bool goUp = false;                   // Flag to indicate whether compare
                                              // value is currently increased or decreased

  //// Placeholder 6 (Task 2.3) //////////////////////////////////////////////
  /* Update compare value variable */
  if (goUp)
  {
    pwmCompareVal += COMPARE_VAL_INC_STEP;
  }
  else
  {
    pwmCompareVal -= COMPARE_VAL_INC_STEP;
  }
  ////////////////////////////////////////////////////////////////////////////

  //// Placeholder 7 (Task 2.3) //////////////////////////////////////////////
  /* Invert the direction of increase/decrease of the duty cycle */
  if (pwmCompareVal <= 0)
  {
    goUp = !goUp;
    pwmCompareVal = 0;
  }
  else if(pwmCompareVal >= PWM_CCR0)
  {
    goUp = !goUp;
    pwmCompareVal = PWM_CCR0;
  }
  ////////////////////////////////////////////////////////////////////////////

  //// Placeholder 8 (Task 2.3) //////////////////////////////////////////////
  /* Update the compare value of Timer A1 (linear increase/decrease) */
  uint16_t pwmCompareValCurr = pwmCompareVal;
  /* BONUS: sine-shaped increase/decrease */
  // uint16_t pwmCompareValCurr =
  //   (uint16_t) (sin((double)(pwmCompareVal)/PWM_CCR0*0.5*M_PI)*PWM_CCR0);
  Timer_A_setCompareValue(TIMER_A1_BASE,
                          TIMER_A_CAPTURECOMPARE_REGISTER_1,
                          pwmCompareValCurr);
  ////////////////////////////////////////////////////////////////////////////
//m2_6e//
//m2_4s//

  //// Placeholder 9 (Task 2.1) //////////////////////////////////////////////
  /* Clear interrupt of timer A0 */
  Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,
                                       TIMER_A_CAPTURECOMPARE_REGISTER_0);
  ////////////////////////////////////////////////////////////////////////////
}
//m2_4e//
