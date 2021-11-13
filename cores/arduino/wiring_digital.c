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

#ifdef __cplusplus
extern "C" {
#endif

void pinMode(pin_size_t ulPin, PinMode ulMode)
{
    PinName p = DIGITAL_TO_PINNAME(ulPin);
    switch (ulMode) {
        case INPUT:
            pin_function(p, GD_PIN_FUNCTION3(PIN_MODE_IN_FLOATING, 0, 0));
            break;
        case INPUT_PULLUP:
            pin_function(p, GD_PIN_FUNCTION3(PIN_MODE_IPU, 0, 0));
            break;
        case INPUT_PULLDOWN:
            pin_function(p, GD_PIN_FUNCTION3(PIN_MODE_IPD, 0, 0));
            break;
        case OUTPUT:
            pin_function(p, GD_PIN_FUNCTION3(PIN_MODE_OUT_PP, PIN_OTYPE_PP, 0));
            break;
#pragma GCC diagnostic ignored "-Wswitch"
        case INPUT_ANALOG: // From PinModeExtension
            pin_function(p, GD_PIN_FUNCTION3(PIN_MODE_AIN, 0, 0));
            break;
#pragma GCC diagnostic ignored "-Wswitch"
        case OUTPUT_OPEN_DRAIN: // From PinModeExtension
            pin_function(p, GD_PIN_FUNCTION3(PIN_MODE_OUT_OD, PIN_OTYPE_OD, 0));
            break;
        default:
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

const uint32_t gpio_port[] = {
#ifdef GPIOA
    GPIOA,
#else
    0,
#endif
#ifdef GPIOB
    GPIOB,
#else
    0,
#endif
#ifdef GPIOC
    GPIOC,
#else
    0,
#endif
#ifdef GPIOD
    GPIOD,
#else
    0,
#endif
#ifdef GPIOE
    GPIOE,
#else
    0,
#endif
#ifdef GPIOF
    GPIOF,
#else
    0,
#endif
#ifdef GPIOG
    GPIOG,
#else
    0,
#endif
#ifdef GPIOH
    GPIOH,
#else
    0,
#endif
#ifdef GPIOI
    GPIOI
#else
    0
#endif
};

const uint32_t gpio_pin[] = {
    GPIO_PIN_0,
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15
};

#ifdef __cplusplus
}
#endif

