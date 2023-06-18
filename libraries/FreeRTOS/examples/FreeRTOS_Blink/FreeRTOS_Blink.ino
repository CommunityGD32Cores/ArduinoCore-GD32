#include <Arduino.h>
#include <FreeRTOS.h>
#include <task.h>
/* LED to blink, change here */
#ifdef PB2
#define LED PB2
#elif defined(PC13)
#define LED PC13
#else
#define LED PA2
#endif
/* We don't need to use dynamically allocated memory (heap), 
 * we can use static FreeRTOS structures and functions! */
#define STACK_SIZE 256
StaticTask_t xBlinkyTaskBuffer;
StackType_t xBlinkyStack[STACK_SIZE];
TaskHandle_t blinkyTaskHandle;

static void BlinkyTask(void* arg) {
    (void) arg; /* unused */
    pinMode(LED, OUTPUT);
    while (true) {
        digitalWrite(LED, HIGH);
        vTaskDelay(100 / portTICK_PERIOD_MS);
        digitalWrite(LED, LOW);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void setup(void)
{
    blinkyTaskHandle = xTaskCreateStatic(
        BlinkyTask,      /* Function that implements the task. */
        "Blinky",         /* Text name for the task. */
        STACK_SIZE,       /* Number of indexes in the xBlinkyStack array. */
        (void *)0,        /* Parameter passed into the task. */
        tskIDLE_PRIORITY, /* Priority at which the task is created. */
        xBlinkyStack,           /* Array to use as the task's stack. */
        &xBlinkyTaskBuffer);    /* Variable to hold the task's data structure. */

    /* start scheduler. should never return. */
    vTaskStartScheduler();
}
void loop(void) { /* never readched */ }
