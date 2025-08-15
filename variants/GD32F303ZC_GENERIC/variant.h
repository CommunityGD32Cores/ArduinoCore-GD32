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
#define PF0 8
#define PF1 9
#define PF2 10
#define PF3 11
#define PF4 12
#define PF5 13
#define PB2 14
#define PF11 15
#define PF12 16
#define PF13 17
#define PF14 18
#define PF15 19
#define PG0 20
#define PG1 21
#define PE7 22
#define PE8 23
#define PE9 24
#define PE10 25
#define PE11 26
#define PE12 27
#define PE13 28
#define PE14 29
#define PE15 30
#define PB10 31
#define PB11 32
#define PB12 33
#define PB13 34
#define PB14 35
#define PB15 36
#define PD8 37
#define PD9 38
#define PD10 39
#define PD11 40
#define PD12 41
#define PD13 42
#define PD14 43
#define PD15 44
#define PG2 45
#define PG3 46
#define PG4 47
#define PG5 48
#define PG6 49
#define PG7 50
#define PG8 51
#define PC6 52
#define PC7 53
#define PC8 54
#define PC9 55
#define PA8 56
#define PA9 57
#define PA10 58
#define PA11 59
#define PA12 60
#define PA13 61
#define PA14 62
#define PA15 63
#define PC10 64
#define PC11 65
#define PC12 66
#define PD0 67
#define PD1 68
#define PD2 69
#define PD3 70
#define PD4 71
#define PD5 72
#define PD6 73
#define PD7 74
#define PG9 75
#define PG10 76
#define PG11 77
#define PG12 78
#define PG13 79
#define PG14 80
#define PG15 81
#define PB3 82
#define PB4 83
#define PB5 84
#define PB6 85
#define PB7 86
#define PB8 87
#define PB9 88
#define PE0 89
#define PE1 90
/* analog pins */
#define PA2 91
#define PC0 92
#define PA3 93
#define PA4 94
#define PC4 95
#define PA6 96
#define PC3 97
#define PF7 98
#define PF8 99
#define PC2 100
#define PB1 101
#define PF9 102
#define PA7 103
#define PA5 104
#define PA0 105
#define PB0 106
#define PF10 107
#define PF6 108
#define PC5 109
#define PA1 110
#define PC1 111

/* digital pins and analog pins number definitions */
#define DIGITAL_PINS_NUM            112
#define ANALOG_PINS_NUM             21
#define ANALOG_PINS_START           PA2
#define ANALOG_PINS_LAST            PC1

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
#define PA7_ALT2 (PA7 | ALT2)
#define PA15_ALT1 (PA15 | ALT1)
#define PB0_ALT1 (PB0 | ALT1)
#define PB0_ALT2 (PB0 | ALT2)
#define PB1_ALT1 (PB1 | ALT1)
#define PB1_ALT2 (PB1 | ALT2)
#define PB3_ALT1 (PB3 | ALT1)
#define PB4_ALT1 (PB4 | ALT1)
#define PB5_ALT1 (PB5 | ALT1)
#define PC0_ALT1 (PC0 | ALT1)
#define PC0_ALT2 (PC0 | ALT2)
#define PC1_ALT1 (PC1 | ALT1)
#define PC1_ALT2 (PC1 | ALT2)
#define PC2_ALT1 (PC2 | ALT1)
#define PC2_ALT2 (PC2 | ALT2)
#define PC3_ALT1 (PC3 | ALT1)
#define PC3_ALT2 (PC3 | ALT2)
#define PC4_ALT1 (PC4 | ALT1)
#define PC5_ALT1 (PC5 | ALT1)
#define PC6_ALT1 (PC6 | ALT1)
#define PC7_ALT1 (PC7 | ALT1)
#define PC8_ALT1 (PC8 | ALT1)
#define PC9_ALT1 (PC9 | ALT1)
#define PC10_ALT1 (PC10 | ALT1)
#define PC11_ALT1 (PC11 | ALT1)

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
