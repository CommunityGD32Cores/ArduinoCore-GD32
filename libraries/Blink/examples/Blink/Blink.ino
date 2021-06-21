// flash the LED2 500ms
void setup()
{
    pinMode(LED2, OUTPUT);
}

// the loop function runs over and over again forever
void loop()
{
    digitalWrite(LED2, HIGH);
    delay(500);
    digitalWrite(LED2, LOW);
    delay(500);
}
