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
#define PA8 0
#define PA9 1
#define PA10 2
#define PA11 3
#define PA12 4
#define PA13 5
#define PA14 6
#define PA15 7
#define PB2 8
#define PB3 9
#define PB4 10
#define PB5 11
#define PB6 12
#define PB7 13
#define PB8 14
#define PB9 15
#define PB10 16
#define PB11 17
#define PB12 18
#define PB13 19
#define PB14 20
#define PB15 21
#define PC6 22
#define PC7 23
#define PC8 24
#define PC9 25
#define PC10 26
#define PC11 27
#define PC12 28
#define PD2 29
#define PF4 30
#define PF5 31
#define PF6 32
#define PF7 33
#define PC13 34
#define PC14 35
#define PC15 36
#define PF0 37
#define PF1 38
/* analog pins */
#define PA0 39
#define PA1 40
#define PA2 41
#define PA3 42
#define PA4 43
#define PA5 44
#define PA6 45
#define PA7 46
#define PB0 47
#define PB1 48
#define PC0 49
#define PC1 50
#define PC2 51
#define PC3 52
#define PC4 53
#define PC5 54

/* digital pins and analog pins number definitions */
#define DIGITAL_PINS_NUM            55
#define ANALOG_PINS_NUM             16
#define ANALOG_PINS_START           PA0
#define ANALOG_PINS_LAST            PC5

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
#define PIN_WIRE_SDA                PB7
#define PIN_WIRE_SCL                PB6
#define USE_I2C                     1

/* TIMER or PWM definitions */
#define TIMER_TONE                  TIMER5
#define TIMER_SERVO                 TIMER6

#define PWM0                        PA0
#define PWM1                        PB5
#define PWM2                        PB6
#define PWM3                        PA6
#define PWM4                        PA11

/* USART definitions */
#define SERIAL_HOWMANY              1
#define USE_USART0_SERIAL           
#define PIN_SERIAL_RX               PA10
#define PIN_SERIAL_TX               PA9
#define SERIAL0_RX                  PA10
#define SERIAL0_TX                  PA9

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
