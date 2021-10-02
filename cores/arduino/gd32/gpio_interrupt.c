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
#elif defined(GD32F3x0)
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

#if defined(GD32F3x0)
static uint8_t convert_gpio_port_to_exti_source(uint32_t gpio_periph) {
    switch(gpio_periph) {
        case GPIOA: return EXTI_SOURCE_GPIOA;
        case GPIOB: return EXTI_SOURCE_GPIOB;
        case GPIOC: return EXTI_SOURCE_GPIOC;
        case GPIOD: return EXTI_SOURCE_GPIOD;
        case GPIOF: return EXTI_SOURCE_GPIOF;
        /* should never happen */
        default: return EXTI_SOURCE_GPIOA;
    }
}

static uint8_t convert_gpio_pin_to_exti_source(uint32_t gpio_pin) {
    switch(gpio_pin) {
        case GPIO_PIN_0: return EXTI_SOURCE_PIN0;
        case GPIO_PIN_1: return EXTI_SOURCE_PIN1;
        case GPIO_PIN_2: return EXTI_SOURCE_PIN2;
        case GPIO_PIN_3: return EXTI_SOURCE_PIN3;
        case GPIO_PIN_4: return EXTI_SOURCE_PIN4;
        case GPIO_PIN_5: return EXTI_SOURCE_PIN5;
        case GPIO_PIN_6: return EXTI_SOURCE_PIN6;
        case GPIO_PIN_7: return EXTI_SOURCE_PIN7;
        case GPIO_PIN_8: return EXTI_SOURCE_PIN8;
        case GPIO_PIN_9: return EXTI_SOURCE_PIN9;
        case GPIO_PIN_10: return EXTI_SOURCE_PIN10;
        case GPIO_PIN_11: return EXTI_SOURCE_PIN11;
        case GPIO_PIN_12: return EXTI_SOURCE_PIN12;
        case GPIO_PIN_13: return EXTI_SOURCE_PIN13;
        case GPIO_PIN_14: return EXTI_SOURCE_PIN14;
        case GPIO_PIN_15: return EXTI_SOURCE_PIN15;
        /* should never happen */
        default: return EXTI_SOURCE_PIN0;
    }
}

#endif

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
    #elif defined(GD32F3x0)
    rcu_periph_clock_enable(RCU_CFGCMP);
    gpio_mode_set(gpio_port[portNum], GPIO_MODE_INPUT, GPIO_PUPD_NONE, gpio_pin[pinNum]);
    #endif

    nvic_irq_enable(gpio_exti_infor[pinNum].irqNum, EXTI_IRQ_PRIO, EXTI_IRQ_SUBPRIO);

    #if defined(GD32F3x0)
    syscfg_exti_line_config(
        convert_gpio_port_to_exti_source(gpio_port[portNum]), 
        convert_gpio_pin_to_exti_source(pinNum));
    #endif

    gpio_exti_source_select(portNum, pinNum);
    exti_init(exti_line, exti_mode, trig_type);
    exti_interrupt_flag_clear(exti_line);

}

void gpio_interrupt_disable(uint32_t pinNum)
{
    int i = 0;
    gpio_exti_infor[pinNum].callback = NULL;
    for(i = 0; i < EXTI_NUMS; i++) {
        if(gpio_exti_infor[pinNum].irqNum == gpio_exti_infor[i].irqNum  && \
                NULL != gpio_exti_infor[i].callback) {
            return ;
        }
    }
    nvic_irq_disable(gpio_exti_infor[pinNum].irqNum);
}


void exti_callbackHandler(uint32_t pinNum)
{
    exti_line_enum linex = (exti_line_enum)BIT(pinNum);
    if(RESET != exti_interrupt_flag_get(linex)) {
        exti_interrupt_flag_clear(linex);
        if(NULL != gpio_exti_infor[pinNum].callback) {
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
    for(i = 5; i < 10; i++) {
        exti_callbackHandler(i);
    }

}

void EXTI10_15_IRQHandler(void)
{
    uint32_t i;
    for(i = 10; i < 16; i++) {
        exti_callbackHandler(i);
    }
}
#elif defined(GD32F3x0)
void EXTI0_1_IRQHandler(void)
{
    uint32_t i;
    for(i = 0; i <= 1; i++) {
        exti_callbackHandler(i);
    }
}

void EXTI2_3_IRQHandler(void)
{
    uint32_t i;
    for(i = 2; i <= 3; i++) {
        exti_callbackHandler(i);
    }
}

void EXTI4_15_IRQHandler(void)
{
    uint32_t i;
    for(i = 4; i <= 15; i++) {
        exti_callbackHandler(i);
    }
}
#endif