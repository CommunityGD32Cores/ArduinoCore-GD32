
#include "Arduino.h"
#include "HardwareTimer.h"

#define   MAX_FREQ  65535

typedef struct {
    PinName pin;
    int32_t count;
} timerPinInfo_t;

static void timerTonePinInit(PinName p, uint32_t frequency, uint32_t duration);
static void tonePeriodElapsedCallback();
static timerPinInfo_t TimerTone_pinInfo = {NC, 0};
static HardwareTimer TimerTone(TIMER_TONE);

// Tone Period elapsed callback in non-blocking mode
static void tonePeriodElapsedCallback()
{
    uint32_t port = gpio_port[GD_PORT_GET(TimerTone_pinInfo.pin)];
    uint8_t pin =  gpio_pin[GD_PIN_GET(TimerTone_pinInfo.pin)];

    if (port != (uint32_t)NC) {
        if (TimerTone_pinInfo.count == -1) {
            gpio_bit_write(port, pin, (bit_status)(1 - (int)gpio_input_bit_get(port, pin)));
        } else if (TimerTone_pinInfo.count != 0) {
            if (TimerTone_pinInfo.count > 0) {
                TimerTone_pinInfo.count--;
            }
            gpio_bit_write(port, pin, (bit_status)(1 - (int)gpio_input_bit_get(port, pin)));
        } else {
            gpio_bit_write(port, pin, (bit_status)0);
        }
    }
}

static void timerTonePinInit(PinName p, uint32_t frequency, uint32_t duration)
{
    uint32_t timFreq = 2 * frequency;

    if (frequency <= MAX_FREQ) {
        if (frequency == 0) {
            TimerTone.stop();
        } else {
            TimerTone_pinInfo.pin = p;

            //Calculate the toggle count
            if (duration > 0) {
                TimerTone_pinInfo.count = ((timFreq * duration) / 1000);
            } else {
                TimerTone_pinInfo.count = -1;
            }

            pin_function(TimerTone_pinInfo.pin, GD_PIN_FUNCTION3(PIN_MODE_OUT_PP, 0, 0));

            TimerTone.setPeriodTime(timFreq, FORMAT_HZ);
            TimerTone.attachInterrupt(tonePeriodElapsedCallback);
            TimerTone.start();
        }
    }
}

// frequency (in hertz) and duration (in milliseconds).
void tone(uint8_t _pin, unsigned int frequency, unsigned long duration)
{
    PinName p = DIGITAL_TO_PINNAME(_pin);

    if (p != NC) {
        if ((TimerTone_pinInfo.pin == NC) || (TimerTone_pinInfo.pin == p)) {
            timerTonePinInit(p, frequency, duration);
        }
    }
}

void noTone(uint8_t _pin)
{
    PinName p = DIGITAL_TO_PINNAME(_pin);
    if ((p != NC) && (TimerTone_pinInfo.pin == p)) {
        TimerTone.stop();
    }
}
