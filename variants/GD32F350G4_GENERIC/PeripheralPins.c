/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
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
};

/* pin descriptions only reference the index in the array, so
 * to get e.g. AF11 one must give it index = 9. provide 
 * convenience macros here.
 * for all other arrays, the value is also equivalent to the index,
 * so there doesn't need to be anything done more.
 */
#define IND_GPIO_AF_0 0
#define IND_GPIO_AF_1 1
#define IND_GPIO_AF_2 2
#define IND_GPIO_AF_3 3
#define IND_GPIO_AF_4 4
#define IND_GPIO_AF_5 5
#define IND_GPIO_AF_6 6
#define IND_GPIO_AF_7 7
#define IND_GPIO_AF_9 8
#define IND_GPIO_AF_11 9

/* GPIO MODE */
const int GD_GPIO_MODE[] = {
    GPIO_MODE_INPUT,             /* 0 */
    GPIO_MODE_OUTPUT,            /* 1 */
    GPIO_MODE_AF,                /* 2 */
    GPIO_MODE_ANALOG,            /* 3 */
};

/* GPIO SPEED */
const int GD_GPIO_SPEED[] = {
    GPIO_OSPEED_2MHZ,             /* 0 */
    GPIO_OSPEED_10MHZ,            /* 1 */
    0,                            /* 2 (unused) */
    GPIO_OSPEED_50MHZ,            /* 3 */
};

/* ADC PinMap */
const PinMap PinMap_ADC[] = {
    {PORTA_0,  ADC, GD_PIN_FUNC_ANALOG_CH(0)},     /* ADC_IN0 */
    {PORTA_1,  ADC, GD_PIN_FUNC_ANALOG_CH(1)},     /* ADC_IN1 */
    {PORTA_2,  ADC, GD_PIN_FUNC_ANALOG_CH(2)},     /* ADC_IN2 */
    {PORTA_3,  ADC, GD_PIN_FUNC_ANALOG_CH(3)},     /* ADC_IN3 */
    {PORTA_4,  ADC, GD_PIN_FUNC_ANALOG_CH(4)},     /* ADC_IN4 */
    {PORTA_5,  ADC, GD_PIN_FUNC_ANALOG_CH(5)},     /* ADC_IN5 */
    {PORTA_6,  ADC, GD_PIN_FUNC_ANALOG_CH(6)},     /* ADC_IN6 */
    {PORTA_7,  ADC, GD_PIN_FUNC_ANALOG_CH(7)},     /* ADC_IN7 */
    {PORTB_0,  ADC, GD_PIN_FUNC_ANALOG_CH(8)},     /* ADC_IN8 */
    {PORTB_1,  ADC, GD_PIN_FUNC_ANALOG_CH(9)},     /* ADC_IN9 */
    {ADC_TEMP, ADC, GD_PIN_FUNC_ANALOG_CH(16)},    /* ADC_IN16 */
    {ADC_VREF, ADC, GD_PIN_FUNC_ANALOG_CH(17)},    /* ADC_IN17 */
    {NC,   NC,    0}
};

/* DAC PinMap */
const PinMap PinMap_DAC[] = {
    {PORTA_4,  DAC, GD_PIN_FUNC_ANALOG_CH(0)},     /* DAC0_OUT */
    {NC,   NC,    0}
};

/* I2C_SDA PinMap */
const PinMap PinMap_I2C_SDA[] = {
    {PORTA_10, I2C0, GD_PIN_FUNCTION4(PIN_MODE_AF, PIN_OTYPE_OD, PIN_PUPD_PULLUP, IND_GPIO_AF_4)}, /* I2C0_SDA */
    {PORTB_7,  I2C0, GD_PIN_FUNCTION4(PIN_MODE_AF, PIN_OTYPE_OD, PIN_PUPD_PULLUP, IND_GPIO_AF_1)}, /* I2C0_SDA */
    {NC,   NC,    0}
};

/* I2C_SCL PinMap */
const PinMap PinMap_I2C_SCL[] = {
    {PORTA_9,  I2C0, GD_PIN_FUNCTION4(PIN_MODE_AF, PIN_OTYPE_OD, PIN_PUPD_PULLUP, IND_GPIO_AF_4)}, /* I2C0_SCL */
    {PORTB_6,  I2C0, GD_PIN_FUNCTION4(PIN_MODE_AF, PIN_OTYPE_OD, PIN_PUPD_PULLUP, IND_GPIO_AF_1)}, /* I2C0_SCL */
    {NC,   NC,    0}
};

/* PWM PinMap */
const PinMap PinMap_PWM[] = {
    {PORTA_0,  TIMER1,  GD_PIN_FUNC_PWM(0, IND_GPIO_AF_2)}, /* TIMER1_CH0,TIMER1_ETI */
    {PORTA_1,  TIMER1,  GD_PIN_FUNC_PWM(1, IND_GPIO_AF_2)}, /* TIMER1_CH1 */
    {PORTA_2,  TIMER14, GD_PIN_FUNC_PWM(0, IND_GPIO_AF_0)}, /* TIMER14_CH0 */
    {PORTA_2,  TIMER1,  GD_PIN_FUNC_PWM(2, IND_GPIO_AF_2)}, /* TIMER1_CH2 */
    {PORTA_3,  TIMER14, GD_PIN_FUNC_PWM(1, IND_GPIO_AF_0)}, /* TIMER14_CH1 */
    {PORTA_3,  TIMER1,  GD_PIN_FUNC_PWM(3, IND_GPIO_AF_2)}, /* TIMER1_CH3 */
    {PORTA_4,  TIMER13, GD_PIN_FUNC_PWM(0, IND_GPIO_AF_4)}, /* TIMER13_CH0 */
    {PORTA_5,  TIMER1,  GD_PIN_FUNC_PWM(0, IND_GPIO_AF_2)}, /* TIMER1_CH0,TIMER1_ETI */
    {PORTA_6,  TIMER2,  GD_PIN_FUNC_PWM(0, IND_GPIO_AF_1)}, /* TIMER2_CH0 */
    {PORTA_6,  TIMER15, GD_PIN_FUNC_PWM(0, IND_GPIO_AF_5)}, /* TIMER15_CH0 */
    {PORTA_7,  TIMER2,  GD_PIN_FUNC_PWM(1, IND_GPIO_AF_1)}, /* TIMER2_CH1 */
    {PORTA_7,  TIMER0,  GD_PIN_FUNC_PWM(0, IND_GPIO_AF_2)}, /* TIMER0_CH0_ON */
    {PORTA_7,  TIMER13, GD_PIN_FUNC_PWM(0, IND_GPIO_AF_4)}, /* TIMER13_CH0 */
    {PORTA_7,  TIMER16, GD_PIN_FUNC_PWM(0, IND_GPIO_AF_5)}, /* TIMER16_CH0 */
    {PORTB_0,  TIMER2,  GD_PIN_FUNC_PWM(2, IND_GPIO_AF_1)}, /* TIMER2_CH2 */
    {PORTB_0,  TIMER0,  GD_PIN_FUNC_PWM(1, IND_GPIO_AF_2)}, /* TIMER0_CH1_ON */
    {PORTB_1,  TIMER13, GD_PIN_FUNC_PWM(0, IND_GPIO_AF_0)}, /* TIMER13_CH0 */
    {PORTB_1,  TIMER2,  GD_PIN_FUNC_PWM(3, IND_GPIO_AF_1)}, /* TIMER2_CH3 */
    {PORTB_1,  TIMER0,  GD_PIN_FUNC_PWM(2, IND_GPIO_AF_2)}, /* TIMER0_CH2_ON */
    {PORTA_9,  TIMER0,  GD_PIN_FUNC_PWM(1, IND_GPIO_AF_2)}, /* TIMER0_CH1 */
    {PORTA_10, TIMER0,  GD_PIN_FUNC_PWM(2, IND_GPIO_AF_2)}, /* TIMER0_CH2 */
    {PORTA_11, TIMER0,  GD_PIN_FUNC_PWM(3, IND_GPIO_AF_2)}, /* TIMER0_CH3 */
    {PORTA_15, TIMER1,  GD_PIN_FUNC_PWM(0, IND_GPIO_AF_2)}, /* TIMER1_CH0,TIMER1_ETI */
    {PORTB_3,  TIMER1,  GD_PIN_FUNC_PWM(1, IND_GPIO_AF_2)}, /* TIMER1_CH1 */
    {PORTB_4,  TIMER2,  GD_PIN_FUNC_PWM(0, IND_GPIO_AF_1)}, /* TIMER2_CH0 */
    {PORTB_5,  TIMER2,  GD_PIN_FUNC_PWM(1, IND_GPIO_AF_1)}, /* TIMER2_CH1 */
    {PORTB_6,  TIMER15, GD_PIN_FUNC_PWM(0, IND_GPIO_AF_2)}, /* TIMER15_CH0_ON */
    {PORTB_7,  TIMER16, GD_PIN_FUNC_PWM(0, IND_GPIO_AF_2)}, /* TIMER16_CH0_ON */
    {NC,   NC,    0}
};

/* UART_TX PinMap */
const PinMap PinMap_UART_TX[] = {
    {PORTA_2,  USART0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_PULLUP, IND_GPIO_AF_1)}, /* USART0_TX */
    {PORTA_9,  USART0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_PULLUP, IND_GPIO_AF_1)}, /* USART0_TX */
    {PORTA_14, USART0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_PULLUP, IND_GPIO_AF_1)}, /* USART0_TX */
    {PORTB_6,  USART0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_PULLUP, IND_GPIO_AF_0)}, /* USART0_TX */
    {NC,   NC,    0}
};

/* UART_RX PinMap */
const PinMap PinMap_UART_RX[] = {
    {PORTA_3,  USART0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_PULLUP, IND_GPIO_AF_1)}, /* USART0_RX */
    {PORTB_0,  USART1, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_PULLUP, IND_GPIO_AF_4)}, /* USART1_RX */
    {PORTA_10, USART0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_PULLUP, IND_GPIO_AF_1)}, /* USART0_RX */
    {PORTA_15, USART0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_PULLUP, IND_GPIO_AF_1)}, /* USART0_RX */
    {PORTB_7,  USART0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_PULLUP, IND_GPIO_AF_0)}, /* USART0_RX */
    {NC,   NC,    0}
};

/* UART_RTS PinMap */
const PinMap PinMap_UART_RTS[] = {
    {PORTA_1,  USART0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_NONE, IND_GPIO_AF_1)}, /* USART0_RTS */
    {PORTA_12, USART0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_NONE, IND_GPIO_AF_1)}, /* USART0_RTS */
    {NC,   NC,    0}
};

/* UART_CTS PinMap */
const PinMap PinMap_UART_CTS[] = {
    {PORTA_0,  USART0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_NONE, IND_GPIO_AF_1)}, /* USART0_CTS */
    {PORTA_11, USART0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_NONE, IND_GPIO_AF_1)}, /* USART0_CTS */
    {NC,   NC,    0}
};

/* SPI_MOSI PinMap */
const PinMap PinMap_SPI_MOSI[] = {
    {PORTA_7,  SPI0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_NONE, IND_GPIO_AF_0)}, /* SPI0_MOSI_I2S0_SD */
    {PORTB_5,  SPI0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_NONE, IND_GPIO_AF_0)}, /* SPI0_MOSI _I2S0_SD */
    {NC,   NC,    0}
};

/* SPI_MISO PinMap */
const PinMap PinMap_SPI_MISO[] = {
    {PORTA_6,  SPI0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_NONE, IND_GPIO_AF_0)}, /* SPI0_MISO_I2S0_MCK */
    {PORTB_4,  SPI0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_NONE, IND_GPIO_AF_0)}, /* SPI0_MISO _I2S0_MCK */
    {NC,   NC,    0}
};

/* SPI_SCLK PinMap */
const PinMap PinMap_SPI_SCLK[] = {
    {PORTA_5,  SPI0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_NONE, IND_GPIO_AF_0)}, /* SPI0_SCK_I2S0_CK */
    {PORTB_3,  SPI0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_NONE, IND_GPIO_AF_0)}, /* SPI0_SCK _I2S0_CK */
    {NC,   NC,    0}
};

/* SPI_SSEL PinMap */
const PinMap PinMap_SPI_SSEL[] = {
    {PORTA_4,  SPI0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_NONE, IND_GPIO_AF_0)}, /* SPI0_NSS_I2S0_WS */
    {PORTA_15, SPI0, GD_PIN_FUNCTION4(PIN_MODE_AF, GPIO_OTYPE_PP, PIN_PUPD_NONE, IND_GPIO_AF_0)}, /* SPI0_NSS_I2S0_WS */
    {NC,   NC,    0}
};

/* CAN_RD PinMap */
const PinMap PinMap_CAN_RD[] = {
    {NC,   NC,    0}
};

/* CAN_TD PinMap */
const PinMap PinMap_CAN_TD[] = {
    {NC,   NC,    0}
};

