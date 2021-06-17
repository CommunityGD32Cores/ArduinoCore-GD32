HardwareTimer Timer_1(TIMER1);

void setup()
{
    pinMode(LED3, OUTPUT);

    Timer_1.setPeriodTime(500, FORMAT_MS);
    Timer_1.attachInterrupt(periodcallback);
    Timer_1.start();
}

// the loop function runs over and over again forever
void loop()
{

}

void periodcallback(void)
{
    digitalToggle(LED3);
}