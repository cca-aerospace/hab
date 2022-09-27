/*!
 * \mainpage This documentation is probably unnecessary for this project.
 */

#include <Arduino.h>
#include <SD.h>

#include "SensorData.hpp"
#include "Sensors.hpp"

SDFile file;
const uint8_t SD_PIN = 4;

void setup () {
    Serial.begin(9600);

    data.begin();

    while (!SD.begin(SD_PIN)) {
        Serial.println(F("failed to initialize SD card"));
    }
    Serial.println(F("initialized SD card"));

    file = SD.open("data.txt", FILE_WRITE);
}

const unsigned long dataReadDelay = 1000;
unsigned long last = 0;
unsigned long current;
unsigned long elapsed;

void loop () {
    current = millis();
    elapsed = current - last;
    
    if (elapsed > dataReadDelay) {
        data.update();
        data.write(file);
    }
}