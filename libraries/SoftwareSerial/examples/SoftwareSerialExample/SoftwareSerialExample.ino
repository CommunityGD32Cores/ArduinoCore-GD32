#include  <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);  //RX , TX

void setup()
{

    mySerial.begin(9600);
    mySerial.print("hello arduino");

}
// the loop function runs over and over again forever
void loop()
{

}
