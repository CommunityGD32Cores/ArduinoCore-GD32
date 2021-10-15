/* mbed Microcontroller Library
 * Copyright (c) 2018 GigaDevice Semiconductor Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Based on mbed-os/hal/mbed_pinmap_common.c
 */
#include "pinmap.h"
#include "PortNames.h"
#include <stdio.h> /* for some debug printfs() below */
//#define GD32F30x_CL

extern const int GD_GPIO_MODE[];
extern const int GD_GPIO_SPEED[];
#if defined(GD32F30x) || defined(GD32F10x)
    extern const int GD_GPIO_REMAP[];
#endif
#if defined(GD32F3x0) || defined(GD32F1x0) || defined(GD32F4xx)
    extern const int GD_GPIO_PULL_UP_DOWN[];
    extern const int GD_GPIO_OUTPUT_MODE[];
    extern const int GD_GPIO_AF[];
#endif

static void gd32_gpio_mode_set(uint32_t gpio_periph, uint32_t mode, uint32_t pin);
uint32_t gpio_clock_enable(uint32_t port_idx);

bool pin_in_pinmap(PinName pin, const PinMap *map)
{
    if(pin != (PinName)NC) {
        while(map->pin != NC) {
            if(map->pin == pin) {
                return true;
            }
            map++;
        }
    }
    return false;
}

/** Configure pin (mode, speed, reamp or af function )
 *
 * @param pin gpio pin name
 * @param function gpio pin mode, speed, remap or af function
 */
void pin_function(PinName pin, int function)
{
    if((PinName)NC == pin) {
        printf("pin name does not exist");
        while(1);
    }

    uint32_t mode   = GD_PIN_MODE_GET(function);
    uint32_t remap  = GD_PIN_REMAP_GET(function);
    uint32_t speed  = GD_PIN_SPEED_GET(function);
    uint32_t port   = GD_PORT_GET(pin);
    uint32_t gd_pin = 1 << GD_PIN_GET(pin);

    uint32_t gpio = gpio_clock_enable(port);

#if defined(GD32F30x)
    gpio_init(gpio, GD_GPIO_MODE[mode], GD_GPIO_SPEED[speed], gd_pin);
    if(remap != 0) {
        rcu_periph_clock_enable(RCU_AF);
        gpio_pin_remap_config(GD_GPIO_REMAP[remap], ENABLE);
    }
#elif defined(GD32F3x0) || defined(GD32F1x0) || defined(GD32F4xx) 
    uint32_t af  =   GD_PIN_AF_GET(function);
    uint32_t output = GD_PIN_OUTPUT_MODE_GET(function);
    uint32_t pull =  GD_PIN_PULL_STATE_GET(function);

    gpio_af_set(gpio, GD_GPIO_AF[af], gd_pin);
    gpio_mode_set(gpio, GD_GPIO_MODE[mode], GD_GPIO_PULL_UP_DOWN[pull], gd_pin);
    if(GD_GPIO_MODE[mode] == GPIO_MODE_OUTPUT || GD_GPIO_MODE[mode] == GPIO_MODE_AF)
        gpio_output_options_set(gpio, GD_GPIO_OUTPUT_MODE[output], GD_GPIO_SPEED[speed], gd_pin);
#endif
}

void pinmap_pinout(PinName pin, const PinMap *map)
{
    if(pin == NC) {
        return;
    }

    while(map->pin != NC) {
        if(map->pin == pin) {
            pin_function(pin, map->function);

            //pin_mode(pin, PullNone);
            return;
        }
        map++;
    }
}

uint32_t pinmap_merge(uint32_t a, uint32_t b)
{
    // both are the same (inc both NC)
    if(a == b) {
        return a;
    }

    // one (or both) is not connected
    if(a == (uint32_t)NC) {
        return b;
    }
    if(b == (uint32_t)NC) {
        return a;
    }

    // mis-match error case
    return (uint32_t)NC;
}

uint32_t pinmap_find_peripheral(PinName pin, const PinMap *map)
{
    while(map->pin != NC) {
        if(map->pin == pin) {
            return map->peripheral;
        }
        map++;
    }
    return (uint32_t)NC;
}

uint32_t pinmap_peripheral(PinName pin, const PinMap *map)
{
    uint32_t peripheral = (uint32_t)NC;

    if(pin == (PinName)NC) {
        return (uint32_t)NC;
    }
    peripheral = pinmap_find_peripheral(pin, map);
    if((uint32_t)NC == peripheral) {  // no mapping available
    }
    return peripheral;
}

uint32_t pinmap_find_function(PinName pin, const PinMap *map)
{
    while(map->pin != NC) {
        if(map->pin == pin) {
            return map->function;
        }
        map++;
    }
    return (uint32_t)NC;
}

uint32_t pinmap_function(PinName pin, const PinMap *map)
{
    uint32_t function = (uint32_t)NC;

    if(pin == (PinName)NC) {
        return (uint32_t)NC;
    }
    function = pinmap_find_function(pin, map);
    if((uint32_t)NC == function) {  // no mapping available
    }
    return function;
}

/** configure gpio pin mode
 *
 * @param gpio_periph gpio port name
 * @param mode gpio pin mode
 * @param pin gpio pin number
 */
static void gd32_gpio_mode_set(uint32_t gpio_periph, uint32_t mode, uint32_t pin)
{
    uint16_t i;
    uint32_t temp_mode = 0U;
    uint32_t reg = 0U;
#if defined(GD32F30x)
    /* GPIO mode configuration */
    temp_mode = (uint32_t)(mode & ((uint32_t)0x0FU));

    /* configure the eight low port pins with GPIO_CTL0 */
    for(i = 0U; i < 8U; i++) {
        if((1U << i) & pin) {
            reg = GPIO_CTL0(gpio_periph);

            /* set the specified pin mode bits */
            reg |= GPIO_MODE_SET(i, temp_mode);

            /* set IPD or IPU */
            if(GPIO_MODE_IPD == mode) {
                /* reset the corresponding OCTL bit */
                GPIO_BC(gpio_periph) = (uint32_t)pin;
            } else {
                /* set the corresponding OCTL bit */
                if(GPIO_MODE_IPU == mode) {
                    GPIO_BOP(gpio_periph) = (uint32_t)pin;
                }
            }

            /* set GPIO_CTL0 register */
            GPIO_CTL0(gpio_periph) = reg;
        }
    }
    /* configure the eight high port pins with GPIO_CTL1 */
    for(i = 8U; i < 16U; i++) {
        if((1U << i) & pin) {
            reg = GPIO_CTL1(gpio_periph);

            /* set the specified pin mode bits */
            reg |= GPIO_MODE_SET(i - 8U, temp_mode);

            /* set IPD or IPU */
            if(GPIO_MODE_IPD == mode) {
                /* reset the corresponding OCTL bit */
                GPIO_BC(gpio_periph) = (uint32_t)pin;
            } else {
                /* set the corresponding OCTL bit */
                if(GPIO_MODE_IPU == mode) {
                    GPIO_BOP(gpio_periph) = (uint32_t)pin;
                }
            }

            /* set GPIO_CTL1 register */
            GPIO_CTL1(gpio_periph) = reg;
        }
    }
#endif /* GD32F30x_CL */

#if defined(GD32F4xx)
    /* get pin mode */
    uint32_t pin_mode = (GPIO_CTL(gpio) >> (gd_pin * 2)) & 0x03;
    if((pin_mode == PIN_MODE_OUTPUT) || (pin_mode == PIN_MODE_ANALOG)) {
        if((mode == OpenDrainNoPull) || (mode == OpenDrainPullUp) || (mode == OpenDrainPullDown)) {
            /* set pin open drain mode */
            GPIO_OMODE(gpio) |= (uint32_t)gd_pin;
        } else {
            /* set pin push pull mode */
            GPIO_OMODE(gpio) &= (uint32_t)(~gd_pin);
        }
    }

    if((mode == OpenDrainPullUp) || (mode == PullUp)) {
        /* clear the specified pin pupd bits */
        GPIO_PUD(gpio) &= ~GPIO_PUPD_MASK(gd_pin);
        /* set the specified pin pull up */
        GPIO_PUD(gpio) |= GPIO_PUPD_SET(gd_pin, GPIO_PUPD_PULLUP);
    } else if((mode == OpenDrainPullDown) || (mode == PullDown)) {
        /* clear the specified pin pupd bits */
        GPIO_PUD(gpio) &= ~GPIO_PUPD_MASK(gd_pin);
        /* set the specified pin pull down */
        GPIO_PUD(gpio) |= GPIO_PUPD_SET(gd_pin, GPIO_PUPD_PULLDOWN);
    } else {
        /* clear the specified pin pupd bits */
        GPIO_PUD(gpio) &= ~GPIO_PUPD_MASK(gd_pin);
        /* set the specified pin pull none */
        GPIO_PUD(gpio) |= GPIO_PUPD_SET(gd_pin, GPIO_PUPD_NONE);

#endif /* GD32F4xx */
    }

/** Enable GPIO clock
 *
 * @param gpio_periph gpio port name
 */
uint32_t gpio_clock_enable(uint32_t port_idx) {
    uint32_t gpio_add = 0;
    switch(port_idx) {
        case PORTA:
            gpio_add = GPIOA;
            rcu_periph_clock_enable(RCU_GPIOA);
            break;
        case PORTB:
            gpio_add = GPIOB;
            rcu_periph_clock_enable(RCU_GPIOB);
            break;
        case PORTC:
            gpio_add = GPIOC;
            rcu_periph_clock_enable(RCU_GPIOC);
            break;
        case PORTD:
            gpio_add = GPIOD;
            rcu_periph_clock_enable(RCU_GPIOD);
            break;
        #ifdef GPIOE
        case PORTE:
            gpio_add = GPIOE;
            rcu_periph_clock_enable(RCU_GPIOE);
            break;
        #endif
        #ifdef GPIOF
        case PORTF:
            gpio_add = GPIOF;
            rcu_periph_clock_enable(RCU_GPIOF);
            break;
        #endif
        #ifdef GPIOG
        case PORTG:
            gpio_add = GPIOG;
            rcu_periph_clock_enable(RCU_GPIOG);
            break;
        #endif
        default:
            printf("port number not exist");
            break;
    }
    return gpio_add;
}
