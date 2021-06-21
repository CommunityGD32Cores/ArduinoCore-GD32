#include <Servo.h>

#define servo_pin 9
#define potentiometer_pin 1
Servo myservo;  // create servo object to control a servo
uint16_t potentiometerValue;
uint16_t servo_angle;


void setup()
{
    // attaches the servo on pin 9 to the servo object
    myservo.attach(servo_pin);
}

void loop()
{
    // reads the value of the potentiometer (value between 0 and 1023)
    potentiometerValue = analogRead( potentiometer_pin);
    // scale it to use it with the servo (value between 0 and 180)
    servo_angle = map(potentiometerValue, 0, 1023, 0, 180);
    // sets the servo position according to the scaled value
    myservo.write( servo_angle);
    // waits for the servo to get there
    delay(15);
}

