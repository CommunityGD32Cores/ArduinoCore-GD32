PWM servo(PWM1);    // PC7

void setup()
{
    pinMode(LED3, OUTPUT);

    // period 500ms, Duty cycle 250ms
    servo.setPeriodCycle(500, 250, FORMAT_MS);
    servo.attachInterrupt(captureCompareCallback);
    servo.start();

}

// the loop function runs over and over again forever
void loop()
{

}

// capture compare interrupt callback function
void captureCompareCallback(void)
{
    digitalToggle(LED3);
}