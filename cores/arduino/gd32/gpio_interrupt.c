#include "gpio_interrupt.h"

#define EXTI_NUMS   (16)

typedef struct {
    IRQn_Type irqNum;
    void (*callback)(void);
} extiConf_t;

extiConf_t gpio_exti_infor[EXTI_NUMS] = {
#if defined(GD32F30x)
    {EXTI0_IRQn,      NULL},
    {EXTI1_IRQn,      NULL},
    {EXTI2_IRQn,      NULL},
    {EXTI3_IRQn,      NULL},
    {EXTI4_IRQn,      NULL},
    {EXTI5_9_IRQn,    NULL},
    {EXTI5_9_IRQn,    NULL},
    {EXTI5_9_IRQn,    NULL},
    {EXTI5_9_IRQn,    NULL},
    {EXTI5_9_IRQn,    NULL},
    {EXTI10_15_IRQn,  NULL},
    {EXTI10_15_IRQn,  NULL},
    {EXTI10_15_IRQn,  NULL},
    {EXTI10_15_IRQn,  NULL},
    {EXTI10_15_IRQn,  NULL},
    {EXTI10_15_IRQn,  NULL}
#elif defined(GD32F3x0) || defined(GD32F1x0)
    {EXTI0_1_IRQn,      NULL},
    {EXTI0_1_IRQn,      NULL},
    {EXTI2_3_IRQn,      NULL},
    {EXTI2_3_IRQn,      NULL},
    {EXTI4_15_IRQn,      NULL},
    {EXTI4_15_IRQn,    NULL},
    {EXTI4_15_IRQn,    NULL},
    {EXTI4_15_IRQn,    NULL},
    {EXTI4_15_IRQn,    NULL},
    {EXTI4_15_IRQn,    NULL},
    {EXTI4_15_IRQn,  NULL},
    {EXTI4_15_IRQn,  NULL},
    {EXTI4_15_IRQn,  NULL},
    {EXTI4_15_IRQn,  NULL},
    {EXTI4_15_IRQn,  NULL},
    {EXTI4_15_IRQn,  NULL}
#endif
};

void gpio_interrupt_enable(uint32_t portNum, uint32_t pinNum, void (*callback)(void), uint32_t mode)
{
    exti_line_enum exti_line = BIT(pinNum);
    exti_mode_enum exti_mode = EXTI_INTERRUPT;
    exti_trig_type_enum  trig_type = mode;
    gpio_exti_infor[pinNum].callback = callback;

    gpio_clock_enable(portNum);
#if defined(GD32F30x)
    rcu_periph_clock_enable(RCU_AF);
    gpio_init(gpio_port[portNum], GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, gpio_pin[pinNum]);
#elif defined(GD32F3x0) || defined(GD32F1x0)
    rcu_periph_clock_enable(RCU_CFGCMP);
    gpio_mode_set(gpio_port[portNum], GPIO_MODE_INPUT, GPIO_PUPD_NONE, gpio_pin[pinNum]);
#endif

    /* some NVIC controllers do not have subprio?!*/
#if defined(GD32E23x)
    nvic_irq_enable(gpio_exti_infor[pinNum].irqNum, EXTI_IRQ_PRIO);
#else
    nvic_irq_enable(gpio_exti_infor[pinNum].irqNum, EXTI_IRQ_PRIO, EXTI_IRQ_SUBPRIO);
#endif
#if defined(GD32F3x0) || defined(GD32F1x0)
    syscfg_exti_line_config(
        (uint8_t) portNum,
        (uint8_t) pinNum);
#elif defined(GD32F30x)
    gpio_exti_source_select(portNum, pinNum);
#endif

    exti_init(exti_line, exti_mode, trig_type);
    exti_interrupt_flag_clear(exti_line);

}

void gpio_interrupt_disable(uint32_t pinNum)
{
    int i = 0;
    gpio_exti_infor[pinNum].callback = NULL;
    for (i = 0; i < EXTI_NUMS; i++) {
        if (gpio_exti_infor[pinNum].irqNum == gpio_exti_infor[i].irqNum  && \
            NULL != gpio_exti_infor[i].callback) {
            return ;
        }
    }
    nvic_irq_disable(gpio_exti_infor[pinNum].irqNum);
}


void exti_callbackHandler(uint32_t pinNum)
{
    exti_line_enum linex = (exti_line_enum)BIT(pinNum);
    if (RESET != exti_interrupt_flag_get(linex)) {
        exti_interrupt_flag_clear(linex);
        if (NULL != gpio_exti_infor[pinNum].callback) {
            gpio_exti_infor[pinNum].callback();
        }
    }
}

#if defined(GD32F30x)
void EXTI0_IRQHandler(void)
{
    exti_callbackHandler(0);
}

void EXTI1_IRQHandler(void)
{
    exti_callbackHandler(1);
}

void EXTI2_IRQHandler(void)
{
    exti_callbackHandler(2);
}

void EXTI3_IRQHandler(void)
{
    exti_callbackHandler(3);
}

void EXTI4_IRQHandler(void)
{
    exti_callbackHandler(4);
}

void EXTI5_9_IRQHandler(void)
{
    uint32_t i;
    for (i = 5; i < 10; i++) {
        exti_callbackHandler(i);
    }

}

void EXTI10_15_IRQHandler(void)
{
    uint32_t i;
    for (i = 10; i < 16; i++) {
        exti_callbackHandler(i);
    }
}
#elif defined(GD32F3x0) || defined(GD32F1x0)
void EXTI0_1_IRQHandler(void)
{
    uint32_t i;
    for (i = 0; i <= 1; i++) {
        exti_callbackHandler(i);
    }
}

void EXTI2_3_IRQHandler(void)
{
    uint32_t i;
    for (i = 2; i <= 3; i++) {
        exti_callbackHandler(i);
    }
}

void EXTI4_15_IRQHandler(void)
{
    uint32_t i;
    for (i = 4; i <= 15; i++) {
        exti_callbackHandler(i);
    }
}
#endif