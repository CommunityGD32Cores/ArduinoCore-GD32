#ifndef _SERVO_H_
#define _SERVO_H_

typedef enum {
    _timer1,                    //Timer1 index
    _Nbr_16timers               //Timer numbers
} timer16_Sequence_t;

#define Servo_VERSION           2     // software version of this library

#define MIN_PULSE_WIDTH       544     // the shortest pulse sent to a servo
#define MAX_PULSE_WIDTH      2400     // the longest pulse sent to a servo
#define DEFAULT_PULSE_WIDTH  1500     // default pulse width when servo is attached
#define REFRESH_INTERVAL    20000     // minumim time to refresh servos in microseconds

#define SERVOS_PER_TIMER       12     // the maximum number of servos controlled by one timer

#define MAX_SERVOS   (_Nbr_16timers  * SERVOS_PER_TIMER)

#define INVALID_SERVO         255     // flag indicating an invalid servo index

typedef struct {
    uint8_t nbr;            // a pin number from 0 to 255
    uint8_t isActive;       // true if this channel is enabled, pin not pulsed if false
} ServoPin_t;

typedef struct {
    ServoPin_t Pin;
    volatile unsigned int ticks;
} servo_t;

/** Class for interfacing with RC servomotors. */
class Servo {
public:
    Servo();
    //uint8_t attach(int pin);
    uint8_t attach(int pin, int min = MIN_PULSE_WIDTH, int max = MAX_PULSE_WIDTH);
    void detach();
    void write(int value);
    void writeMicroseconds(int value);
    int read();
    int readMicroseconds();
    bool attached();
private:
    uint8_t servoIndex;
    int8_t min;
    int8_t max;
};

#endif  /* _SERVO_H_ */
