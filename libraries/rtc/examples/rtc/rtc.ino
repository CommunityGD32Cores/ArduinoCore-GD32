// set current time: year, month, date, hour, minute, second
UTCTimeStruct timeNow = { 2021, 4, 20, 10, 1, 0 }; 

void setup()
{
    pinMode(LED2, OUTPUT);
    Serial.begin(9600);
    rtc.setUTCTime(&timeNow);
    rtc.attachInterrupt(secondIntCallback, INT_SECOND_MODE);
}

// the loop function runs over and over again forever
void loop()
{
    delay(1000);
    //print current time every second
    Serial.print(timeNow.year, DEC);
    Serial.print(' ');
    Serial.print(timeNow.month, DEC);
    Serial.print(' ');
    Serial.print(timeNow.day, DEC);
    Serial.print(' ');
    Serial.print(timeNow.hour, DEC);
    Serial.print(' ');
    Serial.print(timeNow.minutes, DEC);
    Serial.print(' ');
    Serial.println(timeNow.seconds, DEC);
}

// second interrupt callback function
void secondIntCallback(void)
{
    rtc.getUTCTime(&timeNow);
    digitalToggle(LED2);
}
