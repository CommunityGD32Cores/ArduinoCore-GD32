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
#define PB1 2
#define PA9 3
#define PA10 4
#define PA13 5
#define PA14 6
/* analog pins */
#define PA0 7
#define PA3 8
#define PA6 9
#define PA7 10
#define PA1 11
#define PA5 12
#define PA4 13
#define PA2 14

/* digital pins and analog pins number definitions */
#define DIGITAL_PINS_NUM            15
#define ANALOG_PINS_NUM             8
#define ANALOG_PINS_START           PA0
#define ANALOG_PINS_LAST            PA2

/* LED definitions */
#define LED_BUILTIN                 PA4

/* user keys definitions */
#define KEY0                        PA0

/* SPI definitions */
#define PIN_SPI_SS                  PA4
#define PIN_SPI_MOSI                PA7
#define PIN_SPI_MISO                PA6
#define PIN_SPI_SCK                 PA5

/* I2C definitions */
/* I2C0 */
#define HAVE_I2C
#ifndef PIN_WIRE_SDA
#define PIN_WIRE_SDA                PA10
#endif
#ifndef PIN_WIRE_SCL
#define PIN_WIRE_SCL                PA9
#endif

/* I2C1 */
#define HAVE_I2C1
#ifndef PIN_WIRE1_SDA
#define PIN_WIRE1_SDA               PA1
#endif
#ifndef PIN_WIRE1_SCL
#define PIN_WIRE1_SCL               PA0
#endif

/* TIMER or PWM definitions */
#define TIMER_TONE                  TIMER13
#define TIMER_SERVO                 TIMER14

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
