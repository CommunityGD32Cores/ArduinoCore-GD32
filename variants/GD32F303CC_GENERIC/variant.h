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
#define PC13 0
#define PC14 1
#define PC15 2
#define PD0 3
#define PD1 4
#define PA0 5
#define PA1 6
#define PA2 7
#define PA3 8
#define PA4 9
#define PA5 10
#define PA6 11
#define PA7 12
#define PB0 13
#define PB1 14
#define PB2 15
#define PB10 16
#define PB11 17
#define PB12 18
#define PB13 19
#define PB14 20
#define PB15 21
#define PA8 22
#define PA9 23
#define PA10 24
#define PA11 25
#define PA12 26
#define PA13 27
#define PA14 28
#define PA15 29
#define PB3 30
#define PB4 31
#define PB5 32
#define PB6 33
#define PB7 34
#define PB8 35
#define PB9 36

/* digital pins and analog pins number definitions */
#define DIGITAL_PINS_NUM        37
#define ANALOG_PINS_NUM         10
#define ANALOG_PINS_START       PA0
#define ANALOG_PINS_LAST        PB2

/* LED definitions */
#define LED_BUILTIN             PC13
#define LED_GREEN               PC13
#define LED2                    PB2

/* user keys definitions */
#define KEY0                    PB10
#define KEY1                    PB11

/* SPI definitions */
#define PIN_SPI_SS              PA8
#define PIN_SPI_MOSI            PB15
#define PIN_SPI_MISO            PB14
#define PIN_SPI_SCK             PB13

/* I2C definitions */
#define HAVE_I2C
#define PIN_WIRE_SDA            PB9
#define PIN_WIRE_SCL            PB8
#define HAVE_I2C1
#define PIN_WIRE1_SDA           PB11
#define PIN_WIRE1_SCL           PB10

/* TIMER or PWM definitions */
#define TIMER_TONE              TIMER5
#define TIMER_SERVO             TIMER6

#define PWM0                    PA8
#define PWM1                    PA0
#define PWM2                    PA1
#define PWM3                    PA2
#define PWM4                    PB6
#define PWM5                    PB7

/* Serial definitions */
/* "Serial" is by default Serial1 / USART0 */
#ifndef DEFAULT_HWSERIAL_INSTANCE
#define DEFAULT_HWSERIAL_INSTANCE 1
#endif

/* USART0 */
#define HAVE_HWSERIAL1
#define SERIAL0_RX          PA10
#define SERIAL0_TX          PA9

/* USART1*/
#define HAVE_HWSERIAL2
#define SERIAL1_RX          PA3
#define SERIAL1_TX          PA2

/* USART2 */
#define HAVE_HWSERIAL3
#define SERIAL2_RX          PB11
#define SERIAL2_TX          PB10

/* ADC definitions */
#define ADC_RESOLUTION          10
#define DACC_RESOLUTION         12

/* USB definitions */
#define USB_PULLUP                GPIOA
#define USB_PULLUP_PIN            GPIO_PIN_12
#define RCC_AHBPeriph_GPIO_PULLUP RCU_GPIOA

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
