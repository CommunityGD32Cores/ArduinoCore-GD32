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

#include "pins_arduino.h"

#ifdef __cplusplus
extern "C" {
#endif

/* digital pins for pinmap list */
const PinName digital_pins[] = {
    PORTE_2,
    PORTE_3,
    PORTE_4,
    PORTE_5,
    PORTE_6,
    PORTC_13,
    PORTC_14,
    PORTC_15,
    PORTF_0,
    PORTF_1,
    PORTF_2,
    PORTF_3,
    PORTF_4,
    PORTF_5,
    PORTB_2,
    PORTF_11,
    PORTF_12,
    PORTF_13,
    PORTF_14,
    PORTF_15,
    PORTG_0,
    PORTG_1,
    PORTE_7,
    PORTE_8,
    PORTE_9,
    PORTE_10,
    PORTE_11,
    PORTE_12,
    PORTE_13,
    PORTE_14,
    PORTE_15,
    PORTB_10,
    PORTB_11,
    PORTB_12,
    PORTB_13,
    PORTB_14,
    PORTB_15,
    PORTD_8,
    PORTD_9,
    PORTD_10,
    PORTD_11,
    PORTD_12,
    PORTD_13,
    PORTD_14,
    PORTD_15,
    PORTG_2,
    PORTG_3,
    PORTG_4,
    PORTG_5,
    PORTG_6,
    PORTG_7,
    PORTG_8,
    PORTC_6,
    PORTC_7,
    PORTC_8,
    PORTC_9,
    PORTA_8,
    PORTA_9,
    PORTA_10,
    PORTA_11,
    PORTA_12,
    PORTA_13,
    PORTA_14,
    PORTA_15,
    PORTC_11,
    PORTC_12,
    PORTD_0,
    PORTD_1,
    PORTD_2,
    PORTD_3,
    PORTD_4,
    PORTD_5,
    PORTD_6,
    PORTD_7,
    PORTG_9,
    PORTG_10,
    PORTG_11,
    PORTG_12,
    PORTG_13,
    PORTG_14,
    PORTG_15,
    PORTB_3,
    PORTB_4,
    PORTB_5,
    PORTB_6,
    PORTB_7,
    PORTB_8,
    PORTB_9,
    PORTE_0,
    PORTE_1,
    PORTA_2,
    PORTA_5,
    PORTF_10,
    PORTC_2,
    PORTA_6,
    PORTC_5,
    PORTA_4,
    PORTA_0,
    PORTC_4,
    PORTA_1,
    PORTB_0,
    PORTC_0,
    PORTC_3,
    PORTA_3,
    PORTF_7,
    PORTF_6,
    PORTC_1,
    PORTF_8,
    PORTB_1,
    PORTA_7,
    PORTF_9
};

/* analog pins for pinmap list */
const uint32_t analog_pins[] = {
    PA2, //A0
    PA5, //A1
    PF10, //A2
    PC2, //A3
    PA6, //A4
    PC5, //A5
    PA4, //A6
    PA0, //A7
    PC4, //A8
    PA1, //A9
    PB0, //A10
    PC0, //A11
    PC3, //A12
    PA3, //A13
    PF7, //A14
    PF6, //A15
    PC1, //A16
    PF8, //A17
    PB1, //A18
    PA7, //A19
    PF9 //A20
};

#ifdef __cplusplus
}
#endif
