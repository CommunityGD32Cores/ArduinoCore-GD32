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

#ifndef _VARIANT_
#define _VARIANT_

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
/* GPIO pins definitions */
#define PA3  0
#define PA2  1
#define PE4  2
#define PD12 3
#define PB3  4
#define PC7  5
#define PB0  6
#define PB4  7
#define PD11 8
#define PE5  9
#define PA8  10
#define PB15 11
#define PB14 12
#define PB13 13
#define PB9  14
#define PB8  15
#define PE0  16
#define PE1  17
#define PE6  18
#define PC0  19
#define PC1  20
#define PC2  21
#define PC3  22
#define PA0  23
#define PB1  24
#define PE2  25
#define PE7  26
#define PA7  27
#define PC13 28
#define PB11 29
#define PB10 30

/* digital pins and analog pins number definitions */
#define DIGITAL_PINS_NUM        31
#define ANALOG_PINS_NUM         9
#define ANALOG_PINS_START       19
#define ANALOG_PINS_LAST        27

/* LED definitions */
#define LED1                    PE0
#define LED2                    PE1
#define LED3                    PE6

#define LED_BUILTIN             PC13
#define LED_GREEN               PC13

/* user keys definitions */
#define KEY0                    PE2
#define KEY1                    PE7

/* SPI definitions */
#define PIN_SPI_SS              PA8
#define PIN_SPI_MOSI            PB15
#define PIN_SPI_MISO            PB14
#define PIN_SPI_SCK             PB13

/* I2C definitions */
#define PIN_WIRE_SDA            PB9
#define PIN_WIRE_SCL            PB8

/* TIMER or PWM definitions */
#define TIMER_TONE              TIMER5
#define TIMER_SERVO             TIMER6

#define PWM0                    PD12
#define PWM1                    PC7
#define PWM2                    PB0
#define PWM3                    PE5
#define PWM4                    PA8
#define PWM5                    PB15

/* USART definitions */
#define USE_USART1_SERIAL       1
#define PIN_SERIAL_RX           PA3
#define PIN_SERIAL_TX           PA2

/* ADC definitions */
#define ADC_RESOLUTION          10
#define DACC_RESOLUTION         12

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
    /* Port which normally prints to the Arduino Serial Monitor */
    #define SERIAL_PORT_MONITOR     Serial
    /* Hardware serial port, physical RX & TX pins. */
    #define SERIAL_PORT_HARDWARE    Serial1
#endif

#endif /* _VARIANT_ */
