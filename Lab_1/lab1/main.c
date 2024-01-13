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

#include <ESLab1driverLib/driverlib.h>     // TI Platform library (subset)
#include <msp.h>                           // Platform specific header (HW definitions)
#include <stdint.h>                        // Standard Integer Library - datatype definitions
#include <stdio.h>                         // Standard In-/Output definitions/functions

#include "lab1.h"                          // Lab Header file for lab specific defines/declarations

// Global variable defining the number of cycles of the busy loop
uint32_t g_waitcycles = WAIT_CYCLES * WAIT_INIT_MULTI;

/**
 * @brief Embedded Systems (ES) Lab1
 *
 * Base scaffold for the Lab1 in Embedded Systems (ES). The goal of this
 * Lab is that the students get to know the platform MSP432P401R LaunchPad
 * Development Kit. This lab splits into following parts
 * Task 1.1: Flashing an existing application.
 * Task 1.2: Using library functions instead of hard-coded register access
 * Task 1.3: Adding a blinking LED
 * Task 1.4: GPIO Input Polling
 * Task 1.5: Simple UART output
 * Task X: Simple UART input from PC
 */
void main(void)
{
  task_1_1();
  task_1_2();
  task_1_3();
  task_2_2();
  task_2_3();
  task_3();
  task_X();
}
