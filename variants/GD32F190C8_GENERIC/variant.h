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
#define PF0 3
#define PF1 4
#define PB2 5
#define PB10 6
#define PB11 7
#define PB12 8
#define PB13 9
#define PB14 10
#define PB15 11
#define PA8 12
#define PA9 13
#define PA10 14
#define PA11 15
#define PA12 16
#define PA13 17
#define PF6 18
#define PF7 19
#define PA14 20
#define PA15 21
#define PB3 22
#define PB4 23
#define PB5 24
#define PB6 25
#define PB7 26
#define PB8 27
#define PB9 28
/* analog pins */
#define PA0 29
#define PA1 30
#define PA2 31
#define PA3 32
#define PA4 33
#define PA5 34
#define PA6 35
#define PA7 36
#define PB0 37
#define PB1 38

/* digital pins and analog pins number definitions */
#define DIGITAL_PINS_NUM            39
#define ANALOG_PINS_NUM             10
#define ANALOG_PINS_START           PA0
#define ANALOG_PINS_LAST            PB1

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
#define PWM1                        PA1
#define PWM2                        PA2
#define PWM3                        PA3
#define PWM4                        PA4

/* USART definitions */
#define DEFAULT_HWSERIAL_INSTANCE 1           
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
