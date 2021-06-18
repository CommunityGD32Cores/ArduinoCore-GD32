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

#include "pins_arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

/* digital pins for pinmap list */
const PinName digital_pins[] = {
    PORTA_3,    //USART1_RX(TX)                     //Ardunio D0
    PORTA_2,    //USART1_TX(RX)                     //Ardunio D1
    PORTE_4,                                        //Ardunio D2
    PORTD_12,   //TIMER3_CH0(PWM)                   //Ardunio D3
    PORTB_3,                                        //Ardunio D4
    PORTC_7,    //TIMER7_CH1(PWM)                   //Ardunio D5
    PORTB_0,    //TIMER7_CH1_ON(PWM)                //Ardunio D6
    PORTB_4,                                        //Ardunio D7
    PORTD_11,                                       //Ardunio D8
    PORTE_5,    //TIMER8_CH0(PWM)                   //Ardunio D9
    PORTA_8,    //TIMER0_CH0(PWM)/SPI0_NSS(SS)      //Ardunio D10
    PORTB_15,   //TIMER11_CH1(PWM)/SPI1_MOSI(MOSI)  //Ardunio D11
    PORTB_14,   //SPI1_MISO(MISO)                   //Ardunio D12
    PORTB_13,   //SPI1_SCK(SCK)/LED_GREEN           //Ardunio D13
    PORTB_9,    //I2C0_SDA(SDA)                     //Ardunio D14
    PORTB_8,    //I2C0_SCL(SCL)                     //Ardunio D15
    PORTE_0,    //LED_D1
    PORTE_1,    //LED_D2
    PORTE_6,    //LED_D3
    PORTC_0,    //ADC012_IN10                       //Ardunio A0
    PORTC_1,    //ADC012_IN11                       //Ardunio A1
    PORTC_2,    //ADC012_IN12                       //Ardunio A2
    PORTC_3,    //ADC012_IN13                       //Ardunio A3
    PORTA_0,    //ADC01_IN0                         //Ardunio A4
    PORTB_1,    //ADC01_IN9                         //Ardunio A5
    PORTE_2,    //KEY0                              //A6
    PORTE_7,    //KEY1                              //A7
    PORTA_7,    //PA8                               //A8
    PORTC_13,   //(27), LED_BUILTIN
    PORTB_11,   //(28)
};

/* analog pins for pinmap list */
const uint32_t analog_pins[] = {
    19, //A0  //Ardunio A0
    20, //A1  //Ardunio A1
    21, //A2  //Ardunio A2
    22, //A3  //Ardunio A3
    23, //A4  //Ardunio A4
    24, //A5  //Ardunio A5
    25, //A6
    26, //A7
    27, //A8
};
#ifdef __cplusplus
}
#endif
