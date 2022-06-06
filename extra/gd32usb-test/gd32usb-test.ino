#include "KeyboardioHID.h"

void setup()
{
    pinMode(LED2, OUTPUT);

    Serial1.begin(115200);
    Serial1.println("-- Blink demo --");

    while (!Serial) {
        digitalWrite(LED2, HIGH);
        delay(100);
        digitalWrite(LED2, LOW);
        delay(100);
    };

    Serial.println("cdc acm");

    BootKeyboard.begin();
    Mouse.begin();
}

void loop()
{
    Serial1.println("On");
    digitalWrite(LED2, HIGH);
    delay(500);

    Serial.print(".");
    if (Serial.available()) {
        char c = Serial.read();
        Serial.print(c);
    }
    BootKeyboard.press(HID_KEYBOARD_A_AND_A);
    BootKeyboard.sendReport();
    BootKeyboard.releaseAll();
    BootKeyboard.sendReport();

    Mouse.move(10, 0);
    Mouse.sendReport();
    Mouse.releaseAll();
    Mouse.sendReport();

    Serial1.println("Off");
    digitalWrite(LED2, LOW);

    // Delay long enough to see the mouse move.
    delay(500);
}
