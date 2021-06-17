#include <Arduino.h>
#include <Servo.h>
#include "HardwareTimer.h"

static servo_t servos[MAX_SERVOS];          // static array of servo structures
// counter for the servo being pulsed for each timer (or -1 if refresh interval)
static volatile int8_t timerChannel[_Nbr_16timers] = {-1};
static HardwareTimer TimerServo(TIMER_SERVO);

uint8_t ServoCount = 0;         // the total number of attached servos

#define SERVO_MIN() (MIN_PULSE_WIDTH - this->min * 4)   // minimum value in uS for this servo
#define SERVO_MAX() (MAX_PULSE_WIDTH - this->max * 4)   // maximum value in uS for this servo

#define SERVO_TIMER(_timer_id)  ((timer16_Sequence_t)(_timer_id))

/************ static functions common to all instances ***********************/
volatile uint32_t CumulativeCountSinceRefresh = 0;
// Servo  period callback handle
static void Servo_PeriodElapsedCallback()
{
    timer16_Sequence_t timer_id = _timer1;

    if(timerChannel[timer_id] < 0) {
        CumulativeCountSinceRefresh = 0;
    } else {
        if(timerChannel[timer_id] < ServoCount && servos[timerChannel[timer_id]].Pin.isActive == true) {
            digitalWrite(servos[timerChannel[timer_id]].Pin.nbr, LOW);
        }
    }
    timerChannel[timer_id]++;
    if(timerChannel[timer_id] < ServoCount && timerChannel[timer_id] < SERVOS_PER_TIMER) {
        TimerServo.setReloadValue(servos[timerChannel[timer_id]].ticks);
        CumulativeCountSinceRefresh += servos[timerChannel[timer_id]].ticks;
        if(servos[timerChannel[timer_id]].Pin.isActive == true) {
            digitalWrite(servos[timerChannel[timer_id]].Pin.nbr, HIGH);
        }
    } else {
        if(CumulativeCountSinceRefresh + 4 < REFRESH_INTERVAL) {
            TimerServo.setReloadValue(REFRESH_INTERVAL - CumulativeCountSinceRefresh);
        } else {
            TimerServo.refresh();
        }
        timerChannel[timer_id] = -1;
    }
}

// Servo Timer init
static void TimerServoInit()
{
    uint32_t fre = TimerServo.getTimerClkFre();
    TimerServo.setPrescaler(fre / 1000000 - 1);
    TimerServo.setReloadValue(20000);
    TimerServo.attachInterrupt(Servo_PeriodElapsedCallback);
    TimerServo.start();
}

// Check Timer  active status
static bool isTimerActive()
{
    for(uint8_t channel = 0; channel < SERVOS_PER_TIMER; channel++) {
        if(servos[channel].Pin.isActive == true) {
            return true;
        }
    }
    return false;
}

/********************** class servo function *******************************/

Servo::Servo()
{
    if(ServoCount < MAX_SERVOS) {
        this->servoIndex = ServoCount++;
        servos[this->servoIndex].ticks = DEFAULT_PULSE_WIDTH;
    } else {
        this->servoIndex = INVALID_SERVO;
    }
}
// Servo attach a digital pin
// uint8_t Servo::attach(int pin)
// {
//   return this->attach(pin, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
// }

// Servo attach a digital pin
uint8_t Servo::attach(int pin, int min, int max)
{
    if(this->servoIndex < MAX_SERVOS) {
        pinMode(pin, OUTPUT);                      // set servo pin to output
        servos[this->servoIndex].Pin.nbr = pin;
        servos[this->servoIndex].ticks = DEFAULT_PULSE_WIDTH;
        // todo min/max check: abs(min - MIN_PULSE_WIDTH) /4 < 128
        this->min  = (MIN_PULSE_WIDTH - min) / 4; //resolution of min/max is 4 uS
        this->max  = (MAX_PULSE_WIDTH - max) / 4;
        // initialize the timer if it has not already been initialized
        if(isTimerActive() == false) {
            TimerServoInit();
        }
        servos[this->servoIndex].Pin.isActive = true;  // this must be set after the check for isTimerActive
    }
    return this->servoIndex;
}
// Servo datach a digital pin
void Servo::detach()
{
    servos[this->servoIndex].Pin.isActive = false;

    if(isTimerActive() == false) {
        TimerServo.stop();
    }
}

// write an angle to servo
void Servo::write(int value)
{
    if(value < MIN_PULSE_WIDTH) {
        if(value < 0) {
            value = 0;
        } else if(value > 180) {
            value = 180;
        }
        value = map(value, 0, 180, SERVO_MIN(), SERVO_MAX());
    }
    writeMicroseconds(value);
}

// write Microseconds to servo
void Servo::writeMicroseconds(int value)
{
    byte channel = this->servoIndex;
    if((channel < MAX_SERVOS)) {
        if(value < SERVO_MIN()) {
            value = SERVO_MIN();
        } else if(value > SERVO_MAX()) {
            value = SERVO_MAX();
        }
        servos[channel].ticks = value;
    }
}

// return the value as degrees
int Servo::read()
{
    return map(readMicroseconds() + 1, SERVO_MIN(), SERVO_MAX(), 0, 180);
}

// return the value as Microseconds
int Servo::readMicroseconds()
{
    unsigned int pulsewidth;
    if(this->servoIndex != INVALID_SERVO) {
        pulsewidth = servos[this->servoIndex].ticks;
    } else {
        pulsewidth  = 0;
    }
    return pulsewidth;
}

// Check Timer  active status
bool Servo::attached()
{
    return servos[this->servoIndex].Pin.isActive;
}

