#include "FreeRTOS.h"
#include "task.h"
#include "gd32_def.h"
#include <stdio.h>
#include <stdarg.h>

/* implemented per-port */
extern void xPortSysTickHandler (void);

/* will be called by Arduino core's Systick handler */
void osSystickHandler (void) {
  /* Clear overflow flag */
  SysTick->CTRL;

  if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) {
    /* Call tick handler */
    xPortSysTickHandler();
  }
}
StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];


/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
    /* Called if a call to pvPortMalloc() fails because there is insufficient
     * free memory available in the FreeRTOS heap.  pvPortMalloc() is called
     * internally by FreeRTOS API functions that create tasks, queues, software
     * timers, and semaphores.  The size of the FreeRTOS heap is set by the
     * configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */
    taskDISABLE_INTERRUPTS();

    for( ; ; )
    {
    }
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook( TaskHandle_t pxTask,
                                    char * pcTaskName )
{
    ( void ) pcTaskName;
    ( void ) pxTask;

    /* Run time stack overflow checking is performed if
     * configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
     * function is called if a stack overflow is detected. */
    taskDISABLE_INTERRUPTS();

    for( ; ; )
    {
    }
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
    /* This is just a trivial example of an idle hook.  It is called on each
     * cycle of the idle task.  It must *NOT* attempt to block.  In this case the
     * idle task just queries the amount of FreeRTOS heap that remains.  See the
     * memory management section on the https://www.FreeRTOS.org web site for memory
     * management options.  If there is a lot of heap memory free then the
     * configTOTAL_HEAP_SIZE value in FreeRTOSConfig.h can be reduced to free up
     * RAM. */
    #if ( mainCREATE_FULL_DEMO_ONLY == 1 )
        {
            /* Call the idle task processing used by the full demo.  The simple
             * blinky demo does not use the idle task hook. */
            vFullDemoIdleFunction();
        }
    #endif
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
    #if ( mainCREATE_FULL_DEMO_ONLY == 1 )
        {
            vFullDemoTickHookFunction();
        }
    #endif /* mainSELECTED_APPLICATION */
}
/*-----------------------------------------------------------*/

void vAssertCalled( void )
{
    volatile unsigned long looping = 0;

    taskENTER_CRITICAL();
    {
        /* Use the debugger to set ul to a non-zero value in order to step out
         *      of this function to determine why it was called. */
        while( looping == 0LU )
        {
            portNOP();
        }
    }
    taskEXIT_CRITICAL();
}
/*-----------------------------------------------------------*/
void vLoggingPrintf( const char * pcFormat,
                     ... )
{
    va_list arg;

    va_start( arg, pcFormat );
    vprintf( pcFormat, arg );
    va_end( arg );
}

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    StackType_t ** ppxIdleTaskStackBuffer,
                                    uint32_t * pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
 * function then they must be declared static - otherwise they will be allocated on
 * the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
     * state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/*-----------------------------------------------------------*/
void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     StackType_t ** ppxTimerTaskStackBuffer,
                                     uint32_t * pulTimerTaskStackSize )
{
    /* If the buffers to be provided to the Timer task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}


/* runtime stats */
#if configGENERATE_RUN_TIME_STATS == 1
/* Addresses of registers in the Cortex-M debug hardware. */
#define rtsDWT_CYCCNT 			( *( ( unsigned long * ) 0xE0001004 ) )
#define rtsDWT_CONTROL 			( *( ( unsigned long * ) 0xE0001000 ) )
#define rtsSCB_DEMCR 			( *( ( unsigned long * ) 0xE000EDFC ) )
#define rtsTRCENA_BIT			( 0x01000000UL )
#define rtsCOUNTER_ENABLE_BIT	( 0x01UL )

/* Simple shift divide for scaling to avoid an overflow occurring too soon. */
#define runtimeSHIFT_13				13
#define runtimeOVERFLOW_BIT_13		( 1UL << ( 32UL - runtimeSHIFT_13 ) )
static const uint32_t ulPrescaleBits = runtimeSHIFT_13;
static const uint32_t ulOverflowBit = runtimeOVERFLOW_BIT_13;

/*-----------------------------------------------------------*/

void vConfigureTimerForRunTimeStats( void )
{
	/* Enable TRCENA. */
	rtsSCB_DEMCR = rtsSCB_DEMCR | rtsTRCENA_BIT;

	/* Reset counter. */
	rtsDWT_CYCCNT = 0;

	/* Enable counter. */
	rtsDWT_CONTROL = rtsDWT_CONTROL | rtsCOUNTER_ENABLE_BIT;
}
/*-----------------------------------------------------------*/

uint32_t ulGetRunTimeCounterValue( void )
{
static unsigned long ulLastCounterValue = 0UL, ulOverflows = 0;
unsigned long ulValueNow;

	ulValueNow = rtsDWT_CYCCNT;

	/* Has the value overflowed since it was last read. */
	if( ulValueNow < ulLastCounterValue )
	{
		ulOverflows += ulOverflowBit;
	}
	ulLastCounterValue = ulValueNow;

	/* There is no prescale on the counter, so simulate in software. */
	ulValueNow = ( ulValueNow >> ulPrescaleBits ) + ulOverflows;

	return ulValueNow;
}
#endif