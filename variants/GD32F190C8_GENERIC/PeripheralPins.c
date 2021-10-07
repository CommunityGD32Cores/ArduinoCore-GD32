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
const int GD_GPIO_PULL_UP_DOWN[] = {
    GPIO_PUPD_NONE,             /* 0 */
    GPIO_PUPD_PULLUP,           /* 1 */
    GPIO_PUPD_PULLDOWN,         /* 2 */
};

const int GD_GPIO_OUTPUT_MODE[] = {
    GPIO_OTYPE_PP,             /* 0 */
    GPIO_OTYPE_OD,             /* 1 */
};

const int GD_GPIO_AF[] = {
    GPIO_AF_0,             /* 0 */
    GPIO_AF_1,             /* 1 */
    GPIO_AF_2,             /* 2 */
    GPIO_AF_3,             /* 3 */
    GPIO_AF_4,             /* 4 */
    GPIO_AF_5,             /* 5 */
    GPIO_AF_6,             /* 6 */
    GPIO_AF_7,             /* 7 */
    GPIO_AF_9,             /* 8 */
    GPIO_AF_11             /* 9 */
};

/* GPIO MODE */
const int GD_GPIO_MODE[] = {
    GPIO_MODE_INPUT,             /* 0 */
    GPIO_MODE_OUTPUT,            /* 1 */
    GPIO_MODE_AF,                /* 2 */
    GPIO_MODE_ANALOG,            /* 3 */
};

/* GPIO SPEED */
const int GD_GPIO_SPEED[] = {
    GPIO_OSPEED_50MHZ,            /* 0 */
    GPIO_OSPEED_10MHZ,            /* 1 */
    GPIO_OSPEED_2MHZ,             /* 2 */
};

/* ADC PinMap */
const PinMap PinMap_ADC[] = {
    {PORTA_0, ADC, 0 | (0 << 18)},    /* ADC_IN0 */
    {PORTA_1, ADC, 0 | (1 << 18)},    /* ADC_IN1 */
    {PORTA_2, ADC, 0 | (2 << 18)},    /* ADC_IN2 */
    {PORTA_3, ADC, 0 | (3 << 18)},    /* ADC_IN3 */
    {PORTA_4, ADC, 0 | (4 << 18)},    /* ADC_IN4 */
    {PORTA_5, ADC, 0 | (5 << 18)},    /* ADC_IN5 */
    {PORTA_6, ADC, 0 | (6 << 18)},    /* ADC_IN6 */
    {PORTA_7, ADC, 0 | (7 << 18)},    /* ADC_IN7 */
    {PORTB_0, ADC, 0 | (8 << 18)},    /* ADC_IN8 */
    {PORTB_1, ADC, 0 | (9 << 18)},    /* ADC_IN9 */
    {PORTC_0, ADC, 0 | (10 << 18)},   /* ADC_IN10 */
    {PORTC_1, ADC, 0 | (11 << 18)},   /* ADC_IN11 */
    {PORTC_2, ADC, 0 | (12 << 18)},   /* ADC_IN12 */
    {PORTC_3, ADC, 0 | (13 << 18)},   /* ADC_IN13 */
    {PORTC_4, ADC, 0 | (14 << 18)},   /* ADC_IN14 */
    {PORTC_5, ADC, 0 | (15 << 18)},   /* ADC_IN15 */
    {ADC_TEMP, ADC, 0 | (16 << 18)},  /* ADC_IN16 */
    {ADC_VREF, ADC, 0 | (17 << 18)},  /* ADC_IN17 */
    {NC,   NC,    0}
};
/* DAC PinMap */
const PinMap PinMap_DAC[] = {
    #ifdef GD32F350
    {PORTA_4,       DAC, 0 | (0 << 18)},    /* DAC_OUT0 */
    {PORTA_5,       DAC, 0 | (1 << 18)},    /* DAC_OUT1 */
    #endif
    {NC, NC, 0}
};


/* I2C PinMap */
const PinMap PinMap_I2C_SDA[] = {
    {PORTB_7,  I2C0, GD_PIN_FUNCTION4(PIN_MODE_AF, PIN_OTYPE_OD, PIN_PUPD_PULLUP, GPIO_AF_1)},
    {PORTB_9,  I2C0, 6 | (2 << 3)},    /* GPIO_I2C0_REMAP */
    {PORTB_11, I2C1, 6},
    {NC,    NC,    0}
};

const PinMap PinMap_I2C_SCL[] = {
    {PORTB_6,  I2C0, GD_PIN_FUNCTION4(PIN_MODE_AF, PIN_OTYPE_OD, PIN_PUPD_PULLUP, GPIO_AF_1)},
    {PORTB_8,  I2C0, 6 | (2 << 3)},    /* GPIO_I2C0_REMAP */
    {PORTB_10, I2C1, 6},
    {NC,    NC,    0}
};

/* PWM PinMap */
const PinMap PinMap_PWM[] = {
    {PORTA_8,  TIMER0, 7 | (0 << 18)},                      /* TIMER0_CH0 - Default */
    {PORTA_9,  TIMER0, 7 | (1 << 18)},                      /* TIMER0_CH1 - Default */
    {PORTA_10, TIMER0, 7 | (2 << 18)},                      /* TIMER0_CH2 - Default */
    {PORTA_11, TIMER0, 7 | (3 << 18)},                      /* TIMER0_CH3 - Default */
    //{PORTE_9,  TIMER0, 7 | (8 << 3) | (0 << 18)},           /* TIMER0_CH0 - GPIO_TIMER0_FULL_REMAP */
    //{PORTE_11, TIMER0, 7 | (8 << 3) | (1 << 18)},           /* TIMER0_CH1 - GPIO_TIMER0_FULL_REMAP */
    //{PORTE_13, TIMER0, 7 | (8 << 3) | (2 << 18)},           /* TIMER0_CH2 - GPIO_TIMER0_FULL_REMAP */
    //{PORTE_14, TIMER0, 7 | (8 << 3) | (3 << 18)},           /* TIMER0_CH3 - GPIO_TIMER0_FULL_REMAP */

    {PORTA_0,  TIMER1, 7 | (0 << 18)},                      /* TIMER1_CH0_ETI - Default */
    {PORTA_1,  TIMER1, 7 | (1 << 18)},                      /* TIMER1_CH1_ETI - Default */
    {PORTA_2,  TIMER1, 7 | (2 << 18)},                      /* TIMER1_CH2_ETI - Default */
    {PORTA_3,  TIMER1, 7 | (3 << 11)},                      /* TIMER1_CH3_ETI - Default */
    {PORTA_15, TIMER1, 7 | (9 << 3) | (0 << 18)},           /* TIMER1_CH0_ETI- GPIO_TIMER1_PARTIAL_REMAP0 */
    {PORTB_3,  TIMER1, 7 | (9 << 3) | (1 << 18)},           /* TIMER1_CH1 - GPIO_TIMER1_PARTIAL_REMAP0 */
    {PORTB_10, TIMER1, 7 | (10 << 3) | (2 << 18)},          /* TIMER1_CH2 - GPIO_TIMER1_PARTIAL_REMAP1 */
    {PORTB_11, TIMER1, 7 | (10 << 3) | (3 << 18)},          /* TIMER1_CH3 - GPIO_TIMER1_PARTIAL_REMAP1 */
    {PORTA_15, TIMER1, 7 | (11 << 3) | (0 << 18)},          /* TIMER1_CH0_ETI - GPIO_TIMER1_FULL_REMAP */
    {PORTB_3,  TIMER1, 7 | (11 << 3) | (1 << 18)},          /* TIMER1_CH1 - GPIO_TIMER1_FULL_REMAP */
    {PORTB_10, TIMER1, 7 | (11 << 3) | (2 << 18)},          /* TIMER1_CH2 - GPIO_TIMER1_FULL_REMAP */
    {PORTB_11, TIMER1, 7 | (11 << 3) | (3 << 18)},          /* TIMER1_CH3 - GPIO_TIMER1_FULL_REMAP */

    {PORTA_6,  TIMER2, 7 | (0 << 18)},                      /* TIMER2_CH0 - Default */
    {PORTA_7,  TIMER2, 7 | (1 << 18)},                      /* TIMER2_CH1 - Default */
    {PORTB_0,  TIMER2, 7 | (2 << 18)},                      /* TIMER2_CH2 - Default */
    {PORTB_1,  TIMER2, 7 | (3 << 18)},                      /* TIMER2_CH3 - Default */
    {PORTB_4,  TIMER2, 7 | (12 << 3) | (0 << 18)},          /* TIMER2_CH0 - GPIO_TIMER2_PARTIAL_REMAP */
    {PORTB_5,  TIMER2, 7 | (12 << 3) | (1 << 18)},          /* TIMER2_CH1 - GPIO_TIMER2_PARTIAL_REMAP */
    {PORTC_6,  TIMER2, 7 | (13 << 3) | (0 << 18)},          /* TIMER2_CH0 - GPIO_TIMER2_FULL_REMAP */
    {PORTC_7,  TIMER2, 7 | (13 << 3) | (1 << 18)},          /* TIMER2_CH1 - GPIO_TIMER2_FULL_REMAP */
    {PORTC_8,  TIMER2, 7 | (13 << 3) | (2 << 18)},          /* TIMER2_CH2 - GPIO_TIMER2_FULL_REMAP */
    {PORTC_9,  TIMER2, 7 | (13 << 3) | (3 << 18)},          /* TIMER2_CH3 - GPIO_TIMER2_FULL_REMAP */
    {NC,    NC,    0}
};

/* USART PinMap */
const PinMap PinMap_UART_TX[] = {
    {PORTA_9,  USART0, GD_PIN_FUNCTION4(PIN_MODE_AF, PIN_OTYPE_PP, PIN_PUPD_PULLUP, GPIO_AF_1)},
    {PORTB_6,  USART0, GD_PIN_FUNCTION4(PIN_MODE_AF, PIN_OTYPE_PP, PIN_PUPD_PULLUP, GPIO_AF_1)},   /* GPIO_USART0_TX_REMAP */
    {PORTA_2,  USART1, 7},
    //{PORTD_5,  USART1, 7 | (4 << 3)},   /* GPIO_USART1_TX_REMAP */
    // {PORTB_10, UART_2, 7},
    // {PORTC_10, UART_2, 7 | (5 << 3)},   /* GPIO_USART2_TX_PARTIAL_REMAP */
    // {PORTD_8,  UART_2, 7 | (6 << 3)},   /* GPIO_USART2_TX_FULL_REMAP */
    // {PORTC_10, UART_3, 7},
    // {PORTC_12, UART_4, 7},
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RX[] = {
    {PORTA_10, USART0, GD_PIN_FUNCTION4(PIN_MODE_AF, PIN_OTYPE_PP, PIN_PUPD_PULLUP, GPIO_AF_1)},
    {PORTB_7,  USART0, GD_PIN_FUNCTION4(PIN_MODE_AF, PIN_OTYPE_PP, PIN_PUPD_PULLUP, GPIO_AF_1)},
    // {PORTA_3,  UART_1, 1},
    // {PORTD_6,  UART_1, 1 | (4 << 3)},   /* GPIO_USART1_RX_REMAP */
    // {PORTB_11, UART_2, 1},
    // {PORTC_11, UART_2, 1 | (5 << 3)},   /* GPIO_USART2_RX_PARTIAL_REMAP */
    // {PORTD_9,  UART_2, 1 | (6 << 3)},   /* GPIO_USART2_RX_FULL_REMAP */
    // {PORTC_11, UART_3, 1},
    // {PORTD_2,  UART_4, 1},
    {NC,    NC,     0}
};

const PinMap PinMap_UART_RTS[] = {
    {PORTA_12, USART0, 7},
    {PORTA_1,  USART1, 7},
    // {PORTD_4,  UART_1, 7 | (4 << 3)},   /* GPIO_USART1_RTS_REMAP */
    // {PORTB_14, UART_2, 7},
    // {PORTD_12, UART_2, 7 | (6 << 3)},   /* GPIO_USART2_RTS_FULL_REMAP */
    {NC,    NC,    0}
};

const PinMap PinMap_UART_CTS[] = {
    {PORTA_11, USART0, 7},
    {PORTA_0,  USART1, 7},
    // {PORTD_3,  UART_1, 7 | (4 << 3)},   /* GPIO_USART1_CTS_REMAP */
    // {PORTB_13, UART_2, 7},
    // {PORTD_11, UART_2, 7 | (6 << 3)},   /* GPIO_USART2_CTS_FULL_REMAP */
    {NC,    NC,    0}
};

/* SPI PinMap */
const PinMap PinMap_SPI_MOSI[] = {
    {PORTA_7,  SPI0, 7},
    {PORTB_5,  SPI0, 7 | (1 << 3)},    /* GPIO_SPI0_REMAP */
    {PORTB_15, SPI1, 7},
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_MISO[] = {
    {PORTA_6,  SPI0, 7},
    {PORTB_4,  SPI0, 7 | (1 << 3)},    /* GPIO_SPI0_REMAP */
    {PORTB_14, SPI1, 7},
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_SCLK[] = {
    {PORTA_5,  SPI0, 7},
    {PORTB_3,  SPI0, 7 | (1 << 3)},    /* GPIO_SPI0_REMAP */
    {PORTB_13, SPI1, 7},
    {NC,    NC,    0}
};

const PinMap PinMap_SPI_SSEL[] = {
    {PORTA_4,  SPI0, 7},
    {PORTA_15, SPI0, 7 | (1 << 3)},    /* GPIO_SPI0_REMAP */
    {PORTB_12, SPI1, 7},
    {NC,    NC,    0}
};

/* CAN PinMap (no CANs supported) */
const PinMap PinMap_CAN_RD[] = {
    {NC,    NC,    0}
};

const PinMap PinMap_CAN_TD[] = {
    {NC,    NC,    0}
};
const uint32_t gpio_port[] = {
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOF
};
const uint32_t gpio_pin[] = {
    GPIO_PIN_0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15
};
