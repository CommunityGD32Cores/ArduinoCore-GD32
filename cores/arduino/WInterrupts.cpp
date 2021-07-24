/*
  Copyright (c) 2011-2012 Arduino.  All right reserved.

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
#include <api/Interrupts.h>
#include "gpio_interrupt.h"

void attachInterrupt(pin_size_t pin, voidFuncPtr callback, PinStatus mode)
{
    exti_trig_type_enum it_mode;
    PinName pinname = DIGITAL_TO_PINNAME(pin);

    switch(mode) {
        case CHANGE :
            it_mode = EXTI_TRIG_BOTH;
            break;
        case FALLING :
        case LOW :
            it_mode = EXTI_TRIG_FALLING;
            break;
        case RISING :
        case HIGH :
            it_mode = EXTI_TRIG_RISING;
            break;
        default:
            it_mode = EXTI_TRIG_RISING;
            break;
    }
    gpio_interrupt_enable(GD_PORT_GET(pinname), GD_PIN_GET(pinname), callback, it_mode);
}

void detachInterrupt(uint32_t pin)
{
    PinName pinname = DIGITAL_TO_PINNAME(pin);
    gpio_interrupt_disable(GD_PIN_GET(pinname));
}
