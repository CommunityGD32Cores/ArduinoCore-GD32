/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PERIPHERALNAMES_H
#define PERIPHERALNAMES_H

#include "gd32f1x0.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ADC_0 = (int)ADC
} ADCName;

typedef enum {
    DAC_0 = (int)DAC,
} DACName;

typedef enum {
    UART_0 = (int)USART0,
#ifdef USART1
    UART_1 = (int)USART1
#endif
} UARTName;

typedef enum {
    SPI_0 = (int)SPI0,
#ifdef SPI1
    SPI_1 = (int)SPI1
#endif
} SPIName;

typedef enum {
    I2C_0 = (int)I2C0,
#ifdef I2C1
    I2C_1 = (int)I2C1
#endif
} I2CName;

typedef enum {
    PWM_0 = (int)TIMER0,
    PWM_1 = (int)TIMER1,
    PWM_2 = (int)TIMER2,
    PWM_5 = (int)TIMER5
    /* Timer 14..16 are also there, are these usable for PWM? */
} PWMName;


#ifdef __cplusplus
}
#endif

#endif
