/*
    Copyright (c) 2021, CommunityGD32Cores

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
#define PC13 0
#define PC14 1
#define PC15 2
#define PB2 3
#define PB10 4
#define PB11 5
#define PB12 6
#define PB13 7
#define PB14 8
#define PB15 9
#define PA8 10
#define PA9 11
#define PA10 12
#define PA11 13
#define PA12 14
#define PA13 15
#define PA14 16
#define PA15 17
#define PB4 18
#define PB5 19
#define PB6 20
#define PB7 21
#define PB8 22
#define PB9 23
/* analog pins */
#define PA4 24
#define PA6 25
#define PB1 26
#define PA2 27
#define PA7 28
#define PB0 29
#define PA0 30
#define PA1 31
#define PA3 32
#define PA5 33

/* digital pins and analog pins number definitions */
#define DIGITAL_PINS_NUM            34
#define ANALOG_PINS_NUM             10
#define ANALOG_PINS_START           PA4
#define ANALOG_PINS_LAST            PA5

/* alternative pin remappings */
#define PA0_ALT1 (PA0 | ALT1)
#define PA0_ALT2 (PA0 | ALT2)
#define PA1_ALT1 (PA1 | ALT1)
#define PA1_ALT2 (PA1 | ALT2)
#define PA2_ALT1 (PA2 | ALT1)
#define PA2_ALT2 (PA2 | ALT2)
#define PA3_ALT1 (PA3 | ALT1)
#define PA3_ALT2 (PA3 | ALT2)
#define PA4_ALT1 (PA4 | ALT1)
#define PA5_ALT1 (PA5 | ALT1)
#define PA6_ALT1 (PA6 | ALT1)
#define PA7_ALT1 (PA7 | ALT1)
#define PA8_ALT1 (PA8 | ALT1)
#define PA9_ALT1 (PA9 | ALT1)
#define PA10_ALT1 (PA10 | ALT1)
#define PA11_ALT1 (PA11 | ALT1)
#define PA15_ALT1 (PA15 | ALT1)
#define PB0_ALT1 (PB0 | ALT1)
#define PB0_ALT2 (PB0 | ALT2)
#define PB1_ALT1 (PB1 | ALT1)
#define PB1_ALT2 (PB1 | ALT2)
#define PB10_ALT1 (PB10 | ALT1)
#define PB11_ALT1 (PB11 | ALT1)

/* LED definitions */
#define LED_BUILTIN                 PC13

/* user keys definitions */
#define KEY0                        PA0

/* SPI definitions */
#define PIN_SPI_SS                  PA15
#define PIN_SPI_MOSI                PB5
#define PIN_SPI_MISO                PB4
#define PIN_SPI_SCK                 PA5

/* I2C definitions */
/* I2C0 */
#define HAVE_I2C
#ifndef PIN_WIRE_SDA
#define PIN_WIRE_SDA                PB9
#endif
#ifndef PIN_WIRE_SCL
#define PIN_WIRE_SCL                PB8
#endif

/* I2C1 */
#define HAVE_I2C1
#ifndef PIN_WIRE1_SDA
#define PIN_WIRE1_SDA               PB11
#endif
#ifndef PIN_WIRE1_SCL
#define PIN_WIRE1_SCL               PB10
#endif

/* TIMER or PWM definitions */
#define TIMER_TONE                  TIMER5
#define TIMER_SERVO                 TIMER6

#define PWM0                        PA0
#define PWM1                        PA1
#define PWM2                        PA2
#define PWM3                        PA3
#define PWM4                        PA6

/* Serial definitions */
/* "Serial" is by default Serial1 / USART0 */
#ifndef DEFAULT_HWSERIAL_INSTANCE
#define DEFAULT_HWSERIAL_INSTANCE   1
#endif

/* USART0 */
#define HAVE_HWSERIAL1
#ifndef SERIAL0_RX
#define SERIAL0_RX                  PA10
#endif
#ifndef SERIAL0_TX
#define SERIAL0_TX                  PA9
#endif

/* USART1 */
#define HAVE_HWSERIAL2
#ifndef SERIAL1_RX
#define SERIAL1_RX                  PA3
#endif
#ifndef SERIAL1_TX
#define SERIAL1_TX                  PA2
#endif

/* USART2 */
#define HAVE_HWSERIAL3
#ifndef SERIAL2_RX
#define SERIAL2_RX                  PB11
#endif
#ifndef SERIAL2_TX
#define SERIAL2_TX                  PB10
#endif

/* ADC definitions */
#define ADC_RESOLUTION              10

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
