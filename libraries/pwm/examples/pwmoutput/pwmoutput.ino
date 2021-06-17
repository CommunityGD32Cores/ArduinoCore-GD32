PWM servo(PWM1);    //PC7

void setup()
{
    pinMode(LED3, OUTPUT);

    servo.setPeriodCycle(500, 250, FORMAT_MS);
    servo.attachInterrupt(captureCompareCallback);
    servo.start();

}

// the loop function runs over and over again forever
void loop()
{

}

void captureCompareCallback(void)
{
    digitalToggle(LED3);
}