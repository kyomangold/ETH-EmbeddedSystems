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

#ifndef GPIO_H_
#define GPIO_H_

#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define LED1_PORT           GPIO_PORT_P1
#define LED1_PIN            GPIO_PIN0

#define LED2_RED_PORT       GPIO_PORT_P2
#define LED2_RED_PIN        GPIO_PIN0
#define LED2_GREEN_PORT     GPIO_PORT_P2
#define LED2_GREEN_PIN      GPIO_PIN1
#define LED2_BLUE_PORT      GPIO_PORT_P2
#define LED2_BLUE_PIN       GPIO_PIN2

#define BUTTON_S1_PORT      GPIO_PORT_P1
#define BUTTON_S1_PIN       GPIO_PIN1
#define BUTTON_S2_PORT      GPIO_PORT_P1
#define BUTTON_S2_PIN       GPIO_PIN4


void gpio_init(void);

#endif /* GPIO_H_ */
