/*
 * -------------------------------------------
 *    MSP432 DriverLib - v3_21_00_05 
 * -------------------------------------------
 *
 * --COPYRIGHT--,BSD,BSD
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/* Standard Includes */
#include <ESLab1driverLib/gpio.h>
#include <ESLab1driverLib/hw_memmap.h>
#include <stdint.h>

/* DriverLib Includes */

/* DriverLib internal GPIO register offset for optimized performance */
#define OFS_LIB_PAIN  ((uint32_t)&P1->IN - (uint32_t)P1)
#define OFS_LIB_PAOUT   ((uint32_t)&P1->OUT - (uint32_t)P1)
#define OFS_LIB_PADIR   ((uint32_t)&P1->DIR - (uint32_t)P1)
#define OFS_LIB_PAREN   ((uint32_t)&P1->REN - (uint32_t)P1)
#define OFS_LIB_PADS   ((uint32_t)&P1->DS - (uint32_t)P1)
#define OFS_LIB_PASEL0  ((uint32_t)&P1->SEL0 - (uint32_t)P1)
#define OFS_LIB_PASEL1  ((uint32_t)&P1->SEL1 - (uint32_t)P1)
#define OFS_LIB_PAIE    ((uint32_t)&P1->IE - (uint32_t)P1)
#define OFS_LIB_PAIES   ((uint32_t)&P1->IES - (uint32_t)P1)
#define OFS_LIB_PAIFG   ((uint32_t)&P1->IFG - (uint32_t)P1)
#define OFS_LIB_P1IE    ((uint32_t)&P1->IE - (uint32_t)P1)
#define OFS_LIB_P2IE    OFS_LIB_P1IE


static uint32_t GPIO_PORT_TO_BASE[] =
{   0x00,
    (uint32_t)P1,
    (uint32_t)P1+1,
    (uint32_t)P3,
    (uint32_t)P3+1,
    (uint32_t)P5,
    (uint32_t)P5+1,
    (uint32_t)P7,
    (uint32_t)P7+1,
    (uint32_t)P9,
    (uint32_t)P9+1,
    (uint32_t)PJ
    };

void GPIO_setAsOutputPin(uint_fast8_t selectedPort, uint_fast16_t selectedPins)
{
    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    HWREG16(baseAddress + OFS_LIB_PASEL0) &= ~selectedPins;
    HWREG16(baseAddress + OFS_LIB_PASEL1) &= ~selectedPins;
    HWREG16(baseAddress + OFS_LIB_PADIR) |= selectedPins;
}


void GPIO_setAsInputPin(uint_fast8_t selectedPort, uint_fast16_t selectedPins)
{
    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    HWREG16(baseAddress + OFS_LIB_PASEL0) &= ~selectedPins;
    HWREG16(baseAddress + OFS_LIB_PASEL1) &= ~selectedPins;
    HWREG16(baseAddress + OFS_LIB_PADIR) &= ~selectedPins;
    HWREG16(baseAddress + OFS_LIB_PAREN) &= ~selectedPins;
}


void GPIO_setAsPeripheralModuleFunctionOutputPin(uint_fast8_t selectedPort,
        uint_fast16_t selectedPins, uint_fast8_t mode)
{

    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    HWREG16(baseAddress + OFS_LIB_PADIR) |= selectedPins;
    switch (mode)
    {
    case GPIO_PRIMARY_MODULE_FUNCTION:
        HWREG16(baseAddress + OFS_LIB_PASEL0) |= selectedPins;
        HWREG16(baseAddress + OFS_LIB_PASEL1) &= ~selectedPins;
        break;
    case GPIO_SECONDARY_MODULE_FUNCTION:
        HWREG16(baseAddress + OFS_LIB_PASEL0) &= ~selectedPins;
        HWREG16(baseAddress + OFS_LIB_PASEL1) |= selectedPins;
        break;
    case GPIO_TERTIARY_MODULE_FUNCTION:
        HWREG16(baseAddress + OFS_LIB_PASEL0) |= selectedPins;
        HWREG16(baseAddress + OFS_LIB_PASEL1) |= selectedPins;
        break;
    }
}


void GPIO_setAsPeripheralModuleFunctionInputPin(uint_fast8_t selectedPort,
        uint_fast16_t selectedPins, uint_fast8_t mode)
{
    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    HWREG16(baseAddress + OFS_LIB_PADIR) &= ~selectedPins;
    switch (mode)
    {
    case GPIO_PRIMARY_MODULE_FUNCTION:
        HWREG16(baseAddress + OFS_LIB_PASEL0) |= selectedPins;
        HWREG16(baseAddress + OFS_LIB_PASEL1) &= ~selectedPins;
        break;
    case GPIO_SECONDARY_MODULE_FUNCTION:
        HWREG16(baseAddress + OFS_LIB_PASEL0) &= ~selectedPins;
        HWREG16(baseAddress + OFS_LIB_PASEL1) |= selectedPins;
        break;
    case GPIO_TERTIARY_MODULE_FUNCTION:
        HWREG16(baseAddress + OFS_LIB_PASEL0) |= selectedPins;
        HWREG16(baseAddress + OFS_LIB_PASEL1) |= selectedPins;
        break;
    }
}


void GPIO_setOutputHighOnPin(uint_fast8_t selectedPort,
        uint_fast16_t selectedPins)
{

    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    HWREG16(baseAddress + OFS_LIB_PAOUT) |= selectedPins;
}


void GPIO_setOutputLowOnPin(uint_fast8_t selectedPort,
        uint_fast16_t selectedPins)
{

    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    HWREG16(baseAddress + OFS_LIB_PAOUT) &= ~selectedPins;
}


void GPIO_toggleOutputOnPin(uint_fast8_t selectedPort,
        uint_fast16_t selectedPins)
{

    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    HWREG16(baseAddress + OFS_LIB_PAOUT) ^= selectedPins;
}


void GPIO_setAsInputPinWithPullDownResistor(uint_fast8_t selectedPort,
        uint_fast16_t selectedPins)
{

    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    HWREG16(baseAddress + OFS_LIB_PASEL0) &= ~selectedPins;
    HWREG16(baseAddress + OFS_LIB_PASEL1) &= ~selectedPins;

    HWREG16(baseAddress + OFS_LIB_PADIR) &= ~selectedPins;
    HWREG16(baseAddress + OFS_LIB_PAREN) |= selectedPins;
    HWREG16(baseAddress + OFS_LIB_PAOUT) &= ~selectedPins;
}


void GPIO_setAsInputPinWithPullUpResistor(uint_fast8_t selectedPort,
        uint_fast16_t selectedPins)
{

    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    HWREG16(baseAddress + OFS_LIB_PASEL0) &= ~selectedPins;
    HWREG16(baseAddress + OFS_LIB_PASEL1) &= ~selectedPins;
    HWREG16(baseAddress + OFS_LIB_PADIR) &= ~selectedPins;
    HWREG16(baseAddress + OFS_LIB_PAREN) |= selectedPins;
    HWREG16(baseAddress + OFS_LIB_PAOUT) |= selectedPins;
}


uint8_t GPIO_getInputPinValue(uint_fast8_t selectedPort,
        uint_fast16_t selectedPins)
{
    uint_fast16_t inputPinValue;
    uint32_t baseAddress = GPIO_PORT_TO_BASE[selectedPort];

    inputPinValue = HWREG16(baseAddress + OFS_LIB_PAIN) & (selectedPins);

    if (inputPinValue > 0)
        return GPIO_INPUT_PIN_HIGH;
    return GPIO_INPUT_PIN_LOW;
}
