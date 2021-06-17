#include <Servo.h>

#define servo_pin 9
#define potentiometer_pin 1
Servo myservo;  // create servo object to control a servo
uint16_t potentiometerValue;
uint16_t servo_angle;


void setup()
{
    myservo.attach(servo_pin);  // attaches the servo on pin 9 to the servo object
}

void loop()
{
    potentiometerValue = analogRead(
                             potentiometer_pin);            // reads the value of the potentiometer (value between 0 and 1023)
    servo_angle = map(potentiometerValue, 0, 1023, 0,
                      180);     // scale it to use it with the servo (value between 0 and 180)
    myservo.write(
        servo_angle);                  // sets the servo position according to the scaled value
    delay(15);                           // waits for the servo to get there
}

