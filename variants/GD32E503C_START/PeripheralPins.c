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

#include "PeripheralPins.h"
#include "gd32xxyy.h"


/*  void pin_function(PinName pin, int function);
    configure the speed, mode,and remap function of pins
    the parameter function contains the configuration information,show as below
    bit 0:2   gpio mode
    bit 3:8   remap
    bit 9:10  gpio speed
    bit 11:15 adc  /timer channel
*/
const int GD_GPIO_REMAP[] = {
    0x00000000,
    /*TODO: Fill this out */
};

/* GPIO MODE */
const int GD_GPIO_MODE[] = {
    GPIO_MODE_AIN,                /* 0 */
    GPIO_MODE_IN_FLOATING,        /* 1 */
    GPIO_MODE_IPD,                /* 2 */
    GPIO_MODE_IPU,                /* 3 */
    GPIO_MODE_OUT_OD,             /* 4 */
    GPIO_MODE_OUT_PP,             /* 5 */
    GPIO_MODE_AF_OD,              /* 6 */
    GPIO_MODE_AF_PP,              /* 7 */
};

/* GPIO SPEED */
const int GD_GPIO_SPEED[] = {
    GPIO_OSPEED_50MHZ,            /* 0 */
    GPIO_OSPEED_10MHZ,            /* 1 */
    GPIO_OSPEED_2MHZ,             /* 2 */
};

/* ADC PinMap */
const PinMap PinMap_ADC[] = {
    {NC,   NC,    0}
};

/* DAC PinMap */
const PinMap PinMap_DAC[] = {
    {NC, NC, 0}
};


/* I2C PinMap */
const PinMap PinMap_I2C_SDA[] = {
    {NC,    NC,    0}
};

const PinMap PinMap_I2C_SCL[] = {
    {NC,    NC,    0}
};

/* PWM PinMap */
const PinMap PinMap_PWM[] = {
    {NC,    NC,    0}
};

/* USART PinMap */
const PinMap PinMap_UART_TX[] = {
    {PORTA_9,  USART0, 7},
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RX[] = {
    {PORTA_10, USART0, 1},
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RTS[] = {
    {NC,    NC,    0}
};

const PinMap PinMap_UART_CTS[] = {
    {NC,    NC,    0}
};

/* SPI PinMap */
const PinMap PinMap_SPI_MOSI[] = {
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_MISO[] = {
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_SCLK[] = {
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {NC,    NC,    0}
};

/* CAN PinMap */
const PinMap PinMap_CAN_RD[] = {
    {NC,    NC,    0}
};

const PinMap PinMap_CAN_TD[] = {
    {NC,    NC,    0}
};