/*
 * Auto generated Run-Time-Environment Component Configuration File
 *      *** Do not modify ! ***
 *
 * Project: Blinky
 * RTE configuration: Blinky.rteconfig
*/
#ifndef RTE_COMPONENTS_H
#define RTE_COMPONENTS_H

/*
 * Define the Device Header File:
*/
#if defined(GD32F10x)
#define CMSIS_device_header "gd32f10x.h"
#elif defined(GD32F1x0)
#define CMSIS_device_header "gd32f1x0.h"
#elif defined (GD32F20x)
#define CMSIS_device_header "gd32f20x.h"
#elif defined(GD32F3x0)
#define CMSIS_device_header "gd32f3x0.h"
#elif defined(GD32F30x)
#define CMSIS_device_header "gd32f30x.h"
#elif defined(GD32F4xx)
#define CMSIS_device_header "gd32f4xx.h"
#elif defined(GD32F403)
#define CMSIS_device_header "gd32f403.h"
#elif defined(GD32E10X)
#define CMSIS_device_header "gd32e10x.h"
#elif defined(GD32E23x)
#define CMSIS_device_header "gd32e23x.h"
#elif defined(GD32E50X)
#define CMSIS_device_header "gd32e50x.h"
#else
#error "Unknown chip series"
#endif

//#include "cmsis_gcc.h"

/* ARM.FreeRTOS::RTOS.Config.CMSIS RTOS2 */
#define RTE_RTOS_FreeRTOS_CONFIG_RTOS2  /* RTOS FreeRTOS Config for CMSIS RTOS2 API */
/* ARM.FreeRTOS::RTOS.Core.Cortex-M */
#define RTE_RTOS_FreeRTOS_CORE          /* RTOS FreeRTOS Core */
/* ARM.FreeRTOS::RTOS.Event Groups */
#define RTE_RTOS_FreeRTOS_EVENTGROUPS   /* RTOS FreeRTOS Event Groups */
/* ARM.FreeRTOS::RTOS.Heap.Heap_4 */
#define RTE_RTOS_FreeRTOS_HEAP_4        /* RTOS FreeRTOS Heap 4 */
/* ARM.FreeRTOS::RTOS.Timers */
#define RTE_RTOS_FreeRTOS_TIMERS        /* RTOS FreeRTOS Timers */
/* ARM::CMSIS.RTOS2.FreeRTOS.Cortex-M */
#define RTE_CMSIS_RTOS2                 /* CMSIS-RTOS2 */
        #define RTE_CMSIS_RTOS2_FreeRTOS        /* CMSIS-RTOS2 FreeRTOS */

#endif /* RTE_COMPONENTS_H */
