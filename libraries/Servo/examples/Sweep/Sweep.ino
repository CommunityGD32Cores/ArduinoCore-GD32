#include <Servo.h>

#define servo_pin   9
Servo myservo;
int pos = 0;    // variable to store the servo position

void setup()
{
    myservo.attach(servo_pin);  // attaches the servo on pin 9 to the servo object
}

void loop()
{
    for(pos = 0; pos <= 180; pos += 1) {  // goes from 0 degrees to 180 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
    }
    for(pos = 180; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
    }
}

