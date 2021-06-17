// include the SPI library:
#include <SPI.h>

#define arraysize         10

uint32_t send_n = 0, receive_n = 0;
uint8_t master_send_array[arraysize] = {0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA};
uint8_t master_receive_array[arraysize];

void setup()
{
    Serial.begin(9600);

    // initialize PA3(CS) as an output
    pinMode(SS, OUTPUT);

    //set CS pin to high
    digitalWrite(SS, HIGH);
    // start the SPI library:
    SPI.begin();
    // configure SPI speed:
    SPI.setClockDivider(SPI_CLOCK_DIV32);
    //set CS pin to low
    digitalWrite(SS, LOW);
    //start SPI write and read data
    SPI.transfer(master_send_array, master_receive_array, arraysize);
    //set CS pin to high
    digitalWrite(SS, HIGH);
    //close SPI
    SPI.end();
    for(uint8_t n = 0; n < arraysize; n++) {
        Serial.print("\r\nMaster receive data " + String(n));
        Serial.print("\r\n");
        Serial.print(master_receive_array[n]);
    }
}

// the loop function runs over and over again forever
void loop()
{

}