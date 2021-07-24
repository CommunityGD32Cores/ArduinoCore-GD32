/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Arduino.h"
#include "gd32/PinConfigured.h"
#include "gd32/PinNames.h"


/*  PinModes 
#define INPUT         0x0
#define OUTPUT              0x1
#define INPUT_PULLUP        0x2

#define INPUT_FLOATING      INPUT
#define INPUT_PULLDOWN      0x3
#define INPUT_ANALOG        0x4
#define OUTPUT_OPEN_DRAIN   0x5
*/

#ifdef __cplusplus
extern "C" {
#endif

// TODO XXX Arduino API does not currently support INPUT_ANALOG  or OUTPUT_OPEN_DRAIN
//
void pinMode(pin_size_t ulPin, PinMode ulMode)
{
    PinName p = DIGITAL_TO_PINNAME(ulPin);
    switch(ulMode) {
        case INPUT:
            pin_function(p, GD_PIN_FUNCTION3(PIN_MODE_IN_FLOATING, 0, 0));
            break;
        case INPUT_PULLUP:
            pin_function(p, GD_PIN_FUNCTION3(PIN_MODE_IPU, 0, 0));
            break;
        case INPUT_PULLDOWN:
            pin_function(p, GD_PIN_FUNCTION3(PIN_MODE_IPD, 0, 0));
            break;
 // now done in analogRead().
 //       case INPUT_ANALOG:
 //           pin_function(p, GD_PIN_FUNCTION3(PIN_MODE_AIN, 0, 0));
 //           break;
        case OUTPUT:
            pin_function(p, GD_PIN_FUNCTION3(PIN_MODE_OUT_PP, 0, 0));
            break;
 //       case OUTPUT_OPEN_DRAIN:
 //           pin_function(p, GD_PIN_FUNCTION3(PIN_MODE_OUT_OD, 0, 0));
 //           break;
        default:
            //Error_Handler();
            break;
    }
}

void digitalWrite(pin_size_t ulPin, PinStatus status)
{
    PinName pinname = DIGITAL_TO_PINNAME(ulPin);
    uint32_t pin =  gpio_pin[GD_PIN_GET(pinname)];
    uint32_t port = gpio_port[GD_PORT_GET(pinname)];
    gpio_bit_write(port, pin, (bit_status)status);
}

PinStatus digitalRead(pin_size_t ulPin)
{
    PinName pinname = DIGITAL_TO_PINNAME(ulPin);
    uint32_t pin =  gpio_pin[GD_PIN_GET(pinname)];
    uint32_t port = gpio_port[GD_PORT_GET(pinname)];
    return (int)gpio_input_bit_get(port, pin);
}

void digitalToggle(pin_size_t ulPin)
{
    PinName pinname = DIGITAL_TO_PINNAME(ulPin);
    uint32_t pin =  gpio_pin[GD_PIN_GET(pinname)];
    uint32_t port = gpio_port[GD_PORT_GET(pinname)];
    gpio_bit_write(port, pin, (bit_status)(1 - (int)gpio_input_bit_get(port, pin)));
}

#ifdef __cplusplus
}
#endif

