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
#define PE2 0
#define PE3 1
#define PE4 2
#define PE5 3
#define PE6 4
#define PC13 5
#define PC14 6
#define PC15 7
#define PB2 8
#define PE7 9
#define PE8 10
#define PE9 11
#define PE10 12
#define PE11 13
#define PE12 14
#define PE13 15
#define PE14 16
#define PE15 17
#define PB10 18
#define PB11 19
#define PB12 20
#define PB13 21
#define PB14 22
#define PB15 23
#define PD8 24
#define PD9 25
#define PD10 26
#define PD11 27
#define PD12 28
#define PD13 29
#define PD14 30
#define PD15 31
#define PC6 32
#define PC7 33
#define PC8 34
#define PC9 35
#define PA8 36
#define PA9 37
#define PA10 38
#define PA11 39
#define PA12 40
#define PA13 41
#define PA14 42
#define PA15 43
#define PC10 44
#define PC11 45
#define PC12 46
#define PD0 47
#define PD1 48
#define PD2 49
#define PD3 50
/* analog pins */
#define PC3 51
#define PA4 52
#define PA6 53
#define PB1 54
#define PA2 55
#define PA7 56
#define PB0 57
#define PC0 58
#define PA0 59
#define PC1 60
#define PA1 61
#define PA3 62
#define PA5 63
#define PC5 64
#define PC2 65
#define PC4 66

/* digital pins and analog pins number definitions */
#define DIGITAL_PINS_NUM            67
#define ANALOG_PINS_NUM             16
#define ANALOG_PINS_START           PC3
#define ANALOG_PINS_LAST            PC4

/* alternative pin remappings */
#define PA0_ALT1 (PA0 | ALT1)
#define PA1_ALT1 (PA1 | ALT1)
#define PA2_ALT1 (PA2 | ALT1)
#define PA3_ALT1 (PA3 | ALT1)
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
#define PB13_ALT1 (PB13 | ALT1)
#define PB14_ALT1 (PB14 | ALT1)
#define PC0_ALT1 (PC0 | ALT1)
#define PC1_ALT1 (PC1 | ALT1)
#define PC2_ALT1 (PC2 | ALT1)
#define PC3_ALT1 (PC3 | ALT1)
#define PC4_ALT1 (PC4 | ALT1)
#define PC5_ALT1 (PC5 | ALT1)

/* LED definitions */
#define LED_BUILTIN                 PC13

/* user keys definitions */
#define KEY0                        PA0

/* SPI definitions */
#define PIN_SPI_SS                  PA15
#define PIN_SPI_MOSI                PA7
#define PIN_SPI_MISO                PA6
#define PIN_SPI_SCK                 PA5

/* I2C definitions */
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
