#include <Arduino.h>
#include <Mouse.h>

void setup() {
    Serial.begin(9600);
    Mouse.begin();
}

void loop() {
    delay(1000);
    Mouse.click();
    Mouse.release();
    Serial.println(F("clicked once"));
}