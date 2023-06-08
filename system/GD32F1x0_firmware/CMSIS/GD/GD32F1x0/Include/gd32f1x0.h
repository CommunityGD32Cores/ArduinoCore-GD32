/*!
    \file    gd32f1x0.h
    \brief   general definitions for gd32f1x0

    \version 2014-12-26, V1.0.0, platform GD32F1x0(x=3,5)
    \version 2016-01-15, V2.0.0, platform GD32F1x0(x=3,5,7,9)
    \version 2016-04-30, V3.0.0, firmware update for GD32F1x0(x=3,5,7,9)
    \version 2017-06-19, V3.1.0, firmware update for GD32F1x0(x=3,5,7,9)
    \version 2019-11-20, V3.2.0, firmware update for GD32F1x0(x=3,5,7,9)
    \version 2020-09-21, V3.3.0, firmware update for GD32F1x0(x=3,5,7,9)
    \version 2022-08-15, V3.4.0, firmware update for GD32F1x0(x=3,5)
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

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

#ifndef GD32F1X0_H
#define GD32F1X0_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* define GD32F1x0 */
#if !defined (GD32F1x0)
  #define GD32F1x0
#endif /* define GD32F1x0 */
#if !defined (GD32F1x0)
 #error "Please select the target GD32F1x0 device used in your application (in gd32f1x0.h file)"
#endif /* undefine GD32F1x0 tip */

/* define value of high speed crystal oscillator (HXTAL) in Hz */
#if !defined  (HXTAL_VALUE)
#define HXTAL_VALUE    ((uint32_t)8000000)
#endif /* high speed crystal oscillator value */

/* define startup timeout value of high speed crystal oscillator (HXTAL) */
#if !defined  (HXTAL_STARTUP_TIMEOUT)
#define HXTAL_STARTUP_TIMEOUT   ((uint16_t)0x0800)
#endif /* high speed crystal oscillator startup timeout */

/* define value of internal 8MHz RC oscillator (IRC8M) in Hz */
#if !defined  (IRC8M_VALUE) 
#define IRC8M_VALUE  ((uint32_t)8000000)
#endif /* internal 8MHz RC oscillator value */

/* define startup timeout value of internal 8MHz RC oscillator (IRC8M) */
#if !defined  (IRC8M_STARTUP_TIMEOUT)
#define IRC8M_STARTUP_TIMEOUT   ((uint16_t)0x0500)
#endif /* internal 8MHz RC oscillator startup timeout */

/* define value of internal RC oscillator for ADC in Hz */
#if !defined  (IRC14M_VALUE) 
#define IRC14M_VALUE ((uint32_t)14000000)
#endif /* IRC14M for GD32F130_150 */

/* define value of internal 40KHz RC oscillator(IRC40K) in Hz */
#if !defined  (IRC40K_VALUE) 
#define IRC40K_VALUE  ((uint32_t)40000)
#endif /* internal 40KHz RC oscillator value */

/* define value of low speed crystal oscillator (LXTAL)in Hz */
#if !defined  (LXTAL_VALUE) 
#define LXTAL_VALUE  ((uint32_t)32768)
#endif /* low speed crystal oscillator value */

/* GD32F1x0 firmware library version number V3.0 */
#define __GD32F1x0_STDPERIPH_VERSION_MAIN   (0x03) /*!< [31:24] main version     */
#define __GD32F1x0_STDPERIPH_VERSION_SUB1   (0x00) /*!< [23:16] sub1 version     */
#define __GD32F1x0_STDPERIPH_VERSION_SUB2   (0x00) /*!< [15:8]  sub2 version     */
#define __GD32F1x0_STDPERIPH_VERSION_RC     (0x00) /*!< [7:0]  release candidate */ 
#define __GD32F1x0_STDPERIPH_VERSION        ((__GD32F1x0_STDPERIPH_VERSION_MAIN << 24)\
                                            |(__GD32F1x0_STDPERIPH_VERSION_SUB1 << 16)\
                                            |(__GD32F1x0_STDPERIPH_VERSION_SUB2 << 8)\
                                            |(__GD32F1x0_STDPERIPH_VERSION_RC))

/* configuration of the Cortex-M3 processor and core peripherals */
#define __MPU_PRESENT             1        /*!< GD32F1x0 do not provide MPU                              */
#define __NVIC_PRIO_BITS          4        /*!< GD32F1x0 uses 4 bits for the priority levels             */
#define __Vendor_SysTickConfig    0        /*!< set to 1 if different sysTick config is used             */

/* define interrupt number */
typedef enum IRQn
{
    /* Cortex-M3 processor exceptions numbers */
    NonMaskableInt_IRQn          = -14,    /*!< 2 non maskable interrupt                                 */
    MemoryManagement_IRQn        = -12,    /*!< 4 Cortex-M3 memory management interrupt                  */
    BusFault_IRQn                = -11,    /*!< 5 Cortex-M3 bus fault interrupt                          */
    UsageFault_IRQn              = -10,    /*!< 6 Cortex-M3 usage fault interrupt                        */
    SVCall_IRQn                  = -5,     /*!< 11 Cortex-M3 SV call interrupt                           */
    DebugMonitor_IRQn            = -4,     /*!< 12 Cortex-M3 debug monitor interrupt                     */
    PendSV_IRQn                  = -2,     /*!< 14 Cortex-M3 pend SV interrupt                           */
    SysTick_IRQn                 = -1,     /*!< 15 Cortex-M3 system tick interrupt                       */
    /* interruput numbers */
    WWDGT_IRQn                   = 0,      /*!< window watchdog timer interrupt                          */
    LVD_IRQn                     = 1,      /*!< LVD through EXTI line detect interrupt                   */
    RTC_IRQn                     = 2,      /*!< RTC through EXTI line interrupt                          */
    FMC_IRQn                     = 3,      /*!< FMC interrupt                                            */
    RCU_IRQn                     = 4,      /*!< RCU interrupt                                            */
    EXTI0_1_IRQn                 = 5,      /*!< EXTI line 0 and 1 interrupts                             */
    EXTI2_3_IRQn                 = 6,      /*!< EXTI line 2 and 3 interrupts                             */
    EXTI4_15_IRQn                = 7,      /*!< EXTI line 4 to 15 interrupts                             */
    TSI_IRQn                     = 8,      /*!< TSI Interrupt                                            */
    DMA_Channel0_IRQn            = 9,      /*!< DMA channel 0 interrupt                                  */
    DMA_Channel1_2_IRQn          = 10,     /*!< DMA channel 1 and channel 2 interrupts                   */
    DMA_Channel3_4_IRQn          = 11,     /*!< DMA channel 3 and channel 4 interrupts                   */
    ADC_CMP_IRQn                 = 12,     /*!< ADC, CMP0 and CMP1 interrupts                            */
    TIMER0_BRK_UP_TRG_COM_IRQn   = 13,     /*!< TIMER0 break, update, trigger and commutation interrupts */
    TIMER0_Channel_IRQn          = 14,     /*!< TIMER0 channel interrupt                                 */
    TIMER1_IRQn                  = 15,     /*!< TIMER1 interrupt                                         */
    TIMER2_IRQn                  = 16,     /*!< TIMER2 interrupt                                         */
    TIMER5_DAC_IRQn              = 17,     /*!< TIMER5 and DAC interrupts                                */
    TIMER13_IRQn                 = 19,     /*!< TIMER13 interrupt                                        */
    TIMER14_IRQn                 = 20,     /*!< TIMER14 interrupt                                        */
    TIMER15_IRQn                 = 21,     /*!< TIMER15 interrupt                                        */
    TIMER16_IRQn                 = 22,     /*!< TIMER16 interrupt                                        */
    I2C0_EV_IRQn                 = 23,     /*!< I2C0 event interrupt                                     */
    I2C1_EV_IRQn                 = 24,     /*!< I2C1 event interrupt                                     */
    SPI0_IRQn                    = 25,     /*!< SPI0 interrupt                                           */
    SPI1_IRQn                    = 26,     /*!< SPI1 interrupt                                           */
    USART0_IRQn                  = 27,     /*!< USART0 interrupt                                         */
    USART1_IRQn                  = 28,     /*!< USART1 interrupt                                         */
    CEC_IRQn                     = 30,     /*!< CEC interrupt                                            */
    I2C0_ER_IRQn                 = 32,     /*!< I2C0 error interrupt                                     */
    I2C1_ER_IRQn                 = 34,     /*!< I2C1 error interrupt                                     */
    I2C2_EV_IRQn                 = 35,     /*!< I2C2 event interrupt                                     */
    I2C2_ER_IRQn                 = 36,     /*!< I2C2 error interrupt                                     */
    USBD_LP_IRQn                 = 37,     /*!< USBD_LP interrupt                                        */
    USBD_HP_IRQn                 = 38,     /*!< USBD_HP interrupt                                        */
    USBDWakeUp_IRQChannel        = 42,     /*!< USBD_WKUP interrupt                                      */
    DMA_Channel5_6_IRQn          = 48,     /*!< DMA1 channel 5 and channel 6 interrupts                  */
    SPI2_IRQn                    = 51,     /*!< SPI2 global interrupt                                    */ 
} IRQn_Type;

/* includes */
#include "core_cm3.h"
#include "system_gd32f1x0.h"
#include <stdint.h>

/* HACKS for Arduino until we do proper peripheral pins generation */
#define GPIO_AF_11 11
#define GPIO_AF_9 9

/* enum definitions */
typedef enum {DISABLE = 0, ENABLE = !DISABLE} EventStatus, ControlStatus;
#include <stdbool.h>
//typedef enum {FALSE = 0, TRUE = !FALSE} bool;
typedef enum {RESET = 0, SET = !RESET} FlagStatus;
typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrStatus;

/* bit operations */
#define REG32(addr)                  (*(volatile uint32_t *)(uint32_t)(addr))
#define REG16(addr)                  (*(volatile uint16_t *)(uint32_t)(addr))
#define REG8(addr)                   (*(volatile uint8_t *)(uint32_t)(addr))
#define BIT(x)                       ((uint32_t)((uint32_t)0x01U<<(x)))
#define BITS(start, end)             ((0xFFFFFFFFUL << (start)) & (0xFFFFFFFFUL >> (31U - (uint32_t)(end)))) 
#define GET_BITS(regval, start, end) (((regval) & BITS((start),(end))) >> (start))

/* main flash and SRAM memory map */
#define FLASH_BASE            ((uint32_t)0x08000000U)        /*!< main FLASH base address          */
#define SRAM_BASE             ((uint32_t)0x20000000U)        /*!< SRAM base address                */
/* SRAM and peripheral base bit-band region */
#define SRAM_BB_BASE          ((uint32_t)0x22000000U)        /*!< SRAM bit-band base address       */
#define PERIPH_BB_BASE        ((uint32_t)0x42000000U)        /*!< peripheral bit-band base address */
/* peripheral memory map */
#define APB1_BUS_BASE         ((uint32_t)0x40000000U)        /*!< apb1 base address                */
#define APB2_BUS_BASE         ((uint32_t)0x40010000U)        /*!< apb2 base address                */
#define AHB1_BUS_BASE         ((uint32_t)0x40020000U)        /*!< ahb1 base address                */
#define AHB2_BUS_BASE         ((uint32_t)0x48000000U)        /*!< ahb2 base address                */
/* advanced peripheral bus 1 memory map */
#define TIMER_BASE            (APB1_BUS_BASE + 0x00000000U)  /*!< TIMER base address               */
#define SLCD_BASE             (APB1_BUS_BASE + 0x00002400U)  /*!< SLCD base address                */
#define RTC_BASE              (APB1_BUS_BASE + 0x00002800U)  /*!< RTC base address                 */
#define WWDGT_BASE            (APB1_BUS_BASE + 0x00002C00U)  /*!< WWDGT base address               */
#define FWDGT_BASE            (APB1_BUS_BASE + 0x00003000U)  /*!< FWDGT base address               */
#define USART_BASE            (APB1_BUS_BASE + 0x00004400U)  /*!< USART base address               */
#define SPI_BASE              (APB1_BUS_BASE + 0x00003800U)  /*!< SPI base address                 */
#define I2C_BASE              (APB1_BUS_BASE + 0x00005400U)  /*!< I2C base address                 */
#define USBD_BASE             (APB1_BUS_BASE + 0x00005C00U)  /*!< USBD base address                */
#define USBD_RAM_BASE         (APB1_BUS_BASE + 0x00006000U)  /*!< USBD RAM base address            */
#define CAN_BASE              (APB1_BUS_BASE + 0x00006400U)  /*!< CAN base address                 */
#define PMU_BASE              (APB1_BUS_BASE + 0x00007000U)  /*!< PMU base address                 */
#define DAC_BASE              (APB1_BUS_BASE + 0x00007400U)  /*!< DAC base address                 */
#define CEC_BASE              (APB1_BUS_BASE + 0x00007800U)  /*!< CEC base address                 */
#define OPA_BASE              (APB1_BUS_BASE + 0x00007C5CU)  /*!< OPA base address                 */
#define IVREF_BASE            (APB1_BUS_BASE + 0x00007C00U)  /*!< IVREF base address               */
/* advanced peripheral bus 2 memory map */
#define SYSCFG_BASE           (APB2_BUS_BASE + 0x00000000U)  /*!< SYSCFG base address              */
#define CMP_BASE              (APB2_BUS_BASE + 0x0000001CU)  /*!< CMP base address                 */
#define EXTI_BASE             (APB2_BUS_BASE + 0x00000400U)  /*!< EXTI base address                */
#define ADC_BASE              (APB2_BUS_BASE + 0x00002400U)  /*!< ADC base address                 */
/* advanced high performance bus 1 memory map */
#define DMA_BASE              (AHB1_BUS_BASE + 0x00000000U)  /*!< DMA base address                 */
#define DMA_CHANNEL_BASE      (DMA_BASE + 0x00000008U)       /*!< DMA channel base address         */
#define RCU_BASE              (AHB1_BUS_BASE + 0x00001000U)  /*!< RCU base address                 */
#define FMC_BASE              (AHB1_BUS_BASE + 0x00002000U)  /*!< FMC base address                 */
#define CRC_BASE              (AHB1_BUS_BASE + 0x00003000U)  /*!< CRC base address                 */
#define TSI_BASE              (AHB1_BUS_BASE + 0x00004000U)  /*!< TSI base address                 */
/* advanced high performance bus 2 memory map */
#define GPIO_BASE             (AHB2_BUS_BASE + 0x00000000U)  /*!< GPIO base address                 */
/* option byte and debug memory map */
#define OB_BASE               ((uint32_t)0x1FFFF800U)        /*!< OB base address                  */
#define DBG_BASE              ((uint32_t)0xE0042000U)        /*!< DBG base address                 */

/* define marco USE_STDPERIPH_DRIVER */
#if !defined  USE_STDPERIPH_DRIVER
#define USE_STDPERIPH_DRIVER
#endif 
#ifdef USE_STDPERIPH_DRIVER
#include "gd32f1x0_libopt.h"
#endif /* USE_STDPERIPH_DRIVER */

#ifdef __cplusplus
}
#endif
#endif 
