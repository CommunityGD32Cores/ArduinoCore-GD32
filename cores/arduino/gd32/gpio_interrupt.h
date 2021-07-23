#ifndef _GPIO_INTERRUPT_
#define _GPIO_INTERRUPT_

#include "Arduino.h"

#define EXTI_IRQ_PRIO       3
#define EXTI_IRQ_SUBPRIO    0

#ifdef __cplusplus
extern "C" {
#endif

void gpio_interrupt_enable(uint32_t portNum, uint32_t pinNum, void (*callback)(void),
                           uint32_t mode);
void gpio_interrupt_disable(uint32_t pinNum);

#ifdef __cplusplus
}
#endif
#endif /* _GPIO_INTERRUPT_ */
