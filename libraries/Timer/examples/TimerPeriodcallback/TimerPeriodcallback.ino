//define timer object
HardwareTimer Timer_1(TIMER1);

void setup()
{
    pinMode(LED3, OUTPUT);

    // timer period 500ms 
    Timer_1.setPeriodTime(500, FORMAT_MS);
    Timer_1.attachInterrupt(periodcallback);
    Timer_1.start();
}

// the loop function runs over and over again forever
void loop()
{

}

// timer period interrupt callback function
void periodcallback(void)
{
    digitalToggle(LED3);
}