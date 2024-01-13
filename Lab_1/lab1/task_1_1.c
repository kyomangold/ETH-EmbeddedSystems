/*
 * task_1_1.c
 *
 *  Created on: 03 Oct 2020
 *      Author: Kyo
 */

/*

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


#include <msp.h>                              // Platform specific header (HW definitions)
#include <stdint.h>                           // Standard Integer - data type definitions
#include <stdio.h>                            // Standard In-/Output definitions/functions

#include "lab1.h"                             // Lab specific defines/declarations

// These register base address and register address offsets can be found in the MSP432P4xx
// Reference Manual Chapter 10.
#define REGBASEADR  ((uint32_t)(0x40004C00))  // Base addr. of Port 1 configuration register
#define REGOFS_SEL0 ((uint32_t)(0x0000000A))  // Addr. Offset for Select0-Register in Port 1
#define REGOFS_SEL1 ((uint32_t)(0x0000000C))  // Select1 offset in Port 1  conf. reg.
#define REGOFS_DIR  ((uint32_t)(0x00000004))  // Direction offset in Port 1 conf. reg.
#define REGOFS_OUTV ((uint32_t)(0x00000002))  // Output Value offset in Port 1 conf. reg.

void task_1_1(void)
{
  WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;       // Stopping the Watchdog Timer

  uint32_t count = 0;                               // Simple counter variable

  // Configure P1.0 by setting the mode to simple I/O (Select 0 & 1 = 0b00) and direction to
  // output. The configuration information can be found in the MSP432P4xx Reference Manual
  // Chapter 10.
  HWREG16(REGBASEADR + REGOFS_SEL0) &= ~(0x0001);   // Set bit 0 of Select 0 register to 0
  HWREG16(REGBASEADR + REGOFS_SEL1) &= ~(0x0001);   // Set bit 0 of Select 1 register to 0
  HWREG16(REGBASEADR + REGOFS_DIR)  |=  (0x0001);   // Set bit 0 of Direction register to 1

  while(1)
  {
    HWREG16(REGBASEADR + REGOFS_OUTV) ^= (0x0001);  // Toggle Output State of P1.0
    // ---------------------------------------------------------------------------------- //
    //                                    Placeholder 1                                   //
    // ---------------------------------------------------------------------------------- //
    for(count = 0; count < g_waitcycles; count++)   // Busy Loop for Delay
    {
      // ---------------------------------------------------------------------------------//
      //                                  Placeholder 2                                   //
      // ---------------------------------------------------------------------------------//
    }
  }
}
