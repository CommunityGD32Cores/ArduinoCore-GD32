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
#define PF0 0
#define PF1 1
#define PB2 2
#define PA8 3
#define PA9 4
#define PA10 5
#define PA11 6
#define PA12 7
#define PA13 8
#define PA14 9
#define PA15 10
#define PB3 11
#define PB4 12
#define PB5 13
#define PB6 14
#define PB7 15
#define PB8 16
/* analog pins */
#define PB1 17
#define PA6 18
#define PA7 19
#define PA4 20
#define PA3 21
#define PA5 22
#define PB0 23
#define PA2 24
#define PA0 25
#define PA1 26

/* digital pins and analog pins number definitions */
#define DIGITAL_PINS_NUM            27
#define ANALOG_PINS_NUM             10
#define ANALOG_PINS_START           PB1
#define ANALOG_PINS_LAST            PA1

/* LED definitions */
#define LED_BUILTIN                 PB2

/* user keys definitions */
#define KEY0                        PA0

/* SPI definitions */
#define PIN_SPI_SS                  PA15
#define PIN_SPI_MOSI                PB5
#define PIN_SPI_MISO                PB4
#define PIN_SPI_SCK                 PB3

/* I2C definitions */
/* I2C0 */
#define HAVE_I2C
#ifndef PIN_WIRE_SDA
#define PIN_WIRE_SDA                PB7
#endif
#ifndef PIN_WIRE_SCL
#define PIN_WIRE_SCL                PB6
#endif

/* TIMER or PWM definitions */
#define TIMER_TONE                  TIMER5
#define TIMER_SERVO                 TIMER6

#define PWM0                        PA0
#define PWM1                        PA1
#define PWM2                        PA2
#define PWM3                        PA3
#define PWM4                        PA4

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

/* ADC definitions */
#define ADC_RESOLUTION              10
#define DAC_RESOLUTION              12

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
