#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include <stdint.h>

/*-----------------------------------------------------------
* Application specific definitions.
*
* These definitions should be adjusted for your particular hardware and
* application requirements.
*
* THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
* FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
*
* See https://www.freertos.org/a00110.html
*----------------------------------------------------------*/

/* redirect FreeRTOS handler functions to *directly* be the ISR handler functions */
/* saves us from implementing the interrupt ourselves */
#define xPortPendSVHandler PendSV_Handler
#define vPortSVCHandler    SVC_Handler

#define configASSERT_DEFINED                             1
extern void vAssertCalled( void );
#define configASSERT( x )    if( ( x ) == 0 ) vAssertCalled()
#define configQUEUE_REGISTRY_SIZE                        20

#define configUSE_PREEMPTION                             1
#define configUSE_TIME_SLICING                           0
#define configUSE_PORT_OPTIMISED_TASK_SELECTION          0

#define configUSE_IDLE_HOOK                              1
#define configUSE_TICK_HOOK                              1
#define configUSE_DAEMON_TASK_STARTUP_HOOK               0
extern uint32_t SystemCoreClock;
#define configCPU_CLOCK_HZ                               ( ( unsigned long ) SystemCoreClock )
#define configTICK_RATE_HZ                               ( ( TickType_t ) 1000 )
#define configMINIMAL_STACK_SIZE                         ( ( unsigned short ) 128 )
#define configTOTAL_HEAP_SIZE                            ( ( size_t ) ( 512 ) )
#define configMAX_TASK_NAME_LEN                          ( 10 )
#define configUSE_TRACE_FACILITY                         1
#define configUSE_16_BIT_TICKS                           0
#define configIDLE_SHOULD_YIELD                          1
#define configUSE_CO_ROUTINES                            0

#define configMAX_PRIORITIES                             ( 10 )
#define configMAX_CO_ROUTINE_PRIORITIES                  ( 2 )
#define configTIMER_QUEUE_LENGTH                         3
#define configTIMER_TASK_PRIORITY                        ( configMAX_PRIORITIES - 1 )
#define configUSE_COUNTING_SEMAPHORES                    1
#define configSUPPORT_DYNAMIC_ALLOCATION                 0
#define configSUPPORT_STATIC_ALLOCATION                  1
#define configNUM_TX_DESCRIPTORS                         3
#define configSTREAM_BUFFER_TRIGGER_LEVEL_TEST_MARGIN    2

/* don't use trust zone, MPU */
#define configENABLE_TRUSTZONE 0
#define configENABLE_MPU 0
/* enable FPU for those chips having an FPU.. */
#if defined(GD32F4xx) || defined(GD32F403) || defined(GD32F3x0) || defined(GD32F30x) || defined(GD32E10X) || defined(GD32E50X)
#define configENABLE_FPU 1
#else
#define configENABLE_FPU 0
#endif

/* runtime stats generation. can be turned off to save space. */
#define configGENERATE_RUN_TIME_STATS	0
#if configGENERATE_RUN_TIME_STATS == 1
/* Constants related to the generation of run time stats.  Run time stats
are gathered in the full demo, not the blinky demo. */
void vConfigureTimerForRunTimeStats( void );
uint32_t ulGetRunTimeCounterValue( void );
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() vConfigureTimerForRunTimeStats()
#define portGET_RUN_TIME_COUNTER_VALUE() ulGetRunTimeCounterValue()
#endif

/* Set the following definitions to 1 to include the API function, or zero
 * to exclude the API function. */

#define configUSE_MALLOC_FAILED_HOOK              1
#define configUSE_MUTEXES                         1
#define configUSE_RECURSIVE_MUTEXES               1
#define configUSE_TIMERS                          1
#define configTIMER_TASK_STACK_DEPTH              ( configMINIMAL_STACK_SIZE * 2 )

#define INCLUDE_vTaskPrioritySet                  1
#define INCLUDE_uxTaskPriorityGet                 1
#define INCLUDE_vTaskDelete                       1
#define INCLUDE_vTaskCleanUpResources             0
#define INCLUDE_vTaskSuspend                      1
#define INCLUDE_vTaskDelayUntil                   1
#define INCLUDE_vTaskDelay                        1
#define INCLUDE_uxTaskGetStackHighWaterMark       1
#define INCLUDE_uxTaskGetStackHighWaterMark2      1
#define INCLUDE_xTaskGetSchedulerState            1
#define INCLUDE_xTimerGetTimerDaemonTaskHandle    1
#define INCLUDE_xTaskGetIdleTaskHandle            1
#define INCLUDE_xTaskGetHandle                    1
#define INCLUDE_eTaskGetState                     1
#define INCLUDE_xSemaphoreGetMutexHolder          1
#define INCLUDE_xTimerPendFunctionCall            1
#define INCLUDE_xTaskAbortDelay                   1
//#define INCLUDE_xTaskGetCurrentTaskHandle         1

unsigned long ulGetRunTimeCounterValue( void ); /* Prototype of function that returns run time counter. */

#define projCOVERAGE_TEST                       0

#define configKERNEL_INTERRUPT_PRIORITY         255

/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
 * See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY    191 /* equivalent to 0xa0, or priority 5. */
#define configMAC_INTERRUPT_PRIORITY            5

/* Prototype for the function used to print out.  In this case it prints to the
 |     10 console before the network is connected then a UDP port after the network has
 |      9 connected. */
extern void vLoggingPrintf( const char * pcFormatString,
                            ... );

#ifdef HEAP3
    #define xPortGetMinimumEverFreeHeapSize    ( x )
    #define xPortGetFreeHeapSize               ( x )
#endif

#endif /* FREERTOS_CONFIG_H */