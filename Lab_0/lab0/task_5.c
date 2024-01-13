/*
 * task_5.c
 *
 *  Created on: 26 Sep 2020
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
 * Copyright Author:  Romain Jacob
 */

#include "lab0.h"                               // Lab specific defines/declarations

#define LED_RED         (0x01)
#define RGB_RED         (0x02)
#define RGB_GREEN       (0x04)
#define RGB_BLUE        (0x08)

void task_5(void)
{
  // Declare a delay counter
  volatile uint32_t i;

  // Define a toggling counter
  uint32_t counter = 0;

  /*
   * Definition of activeLED
   *
   * activeLED : (MSB)  _____  _____  _____  _____    ________  _________  _______  _______  (LSB)
   *                    **     unused bits      **    RGB_BLUE  RGB_GREEN  RGB_RED  LED_RED
   *
   * Set the bits in activeLED to toggle the corresponding LEDs
   */

/* activeLED definitions that toggles both LED_RED and RGB_RED */
  //  uint8_t activeLED = 3;                    // Using decimal number
  //  uint8_t activeLED = 0x03;                 // Using an hexadecimal number
  //  uint8_t activeLED = 0b00000011;           // Using a binary number
  //  uint8_t activeLED = LED_RED | RGB_RED;    // Using a macro

/* activeLED definitions that toggles all LEDs */
  //  uint8_t activeLED = 15;                    // Using decimal number
  //  uint8_t activeLED = 0x0f;                  // Using an hexadecimal number
  //  uint8_t activeLED = 0b00001111;                 // Using a binary number
  //  uint8_t activeLED = LED_RED | RGB_RED | RGB_GREEN | RGB_BLUE;    // Using a macro

  uint8_t activeLED = 0;

  while(1)
  {
    /* using if statements*/
    if(counter%3 == 0 ){
        activeLED = LED_RED | RGB_RED;
    }
    else if(counter%3 == 1){
        activeLED = LED_RED | RGB_GREEN;
    }
    else if(counter%3 == 2){
        activeLED = LED_RED | RGB_BLUE;
    }

    /* using shift operators */
    activeLED = LED_RED | (1 << (counter%3 + 1) );


    // Toggle an LED
    toggle(activeLED);

    // Delay of ~333ms
    for (i = 0; i < 100000; i++);

    // Toggle an LED
    toggle(activeLED);

    // Delay of ~333ms
    for (i = 0; i < 100000; i++);

    // Print counter value and increment it
    uart_println("counter = %u",counter);
    counter += 1;
  }
}
