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
#define PC6 10
#define PC7 11
#define PC8 12
#define PC9 13
#define PA8 14
#define PA9 15
#define PA10 16
#define PA11 17
#define PA12 18
#define PA13 19
#define PA14 20
#define PA15 21
#define PC10 22
#define PC11 23
#define PC12 24
#define PD2 25
#define PB3 26
#define PB4 27
#define PB5 28
#define PB6 29
#define PB7 30
#define PB8 31
#define PB9 32
/* analog pins */
#define PA4 33
#define PB0 34
#define PC1 35
#define PC3 36
#define PA0 37
#define PB1 38
#define PA7 39
#define PA5 40
#define PC2 41
#define PC5 42
#define PC0 43
#define PA3 44
#define PA6 45
#define PA2 46
#define PA1 47
#define PC4 48

/* digital pins and analog pins number definitions */
#define DIGITAL_PINS_NUM            49
#define ANALOG_PINS_NUM             16
#define ANALOG_PINS_START           PA4
#define ANALOG_PINS_LAST            PC4

/* LED definitions */
#define LED_BUILTIN                 PC13

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

/* UART3 */
#define HAVE_HWSERIAL1
#ifndef SERIAL0_RX
#define SERIAL0_RX                  PC11
#endif
#ifndef SERIAL0_TX
#define SERIAL0_TX                  PC10
#endif

/* UART4 */
#define HAVE_HWSERIAL2
#ifndef SERIAL1_RX
#define SERIAL1_RX                  PD2
#endif
#ifndef SERIAL1_TX
#define SERIAL1_TX                  PC12
#endif

/* USART0 */
#define HAVE_HWSERIAL3
#ifndef SERIAL2_RX
#define SERIAL2_RX                  PA10
#endif
#ifndef SERIAL2_TX
#define SERIAL2_TX                  PA9
#endif

/* USART1 */
#define HAVE_HWSERIAL4
#ifndef SERIAL3_RX
#define SERIAL3_RX                  PA3
#endif
#ifndef SERIAL3_TX
#define SERIAL3_TX                  PA2
#endif

/* USART2 */
#define HAVE_HWSERIAL5
#ifndef SERIAL4_RX
#define SERIAL4_RX                  PB11
#endif
#ifndef SERIAL4_TX
#define SERIAL4_TX                  PB10
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