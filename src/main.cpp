/*!
 * \mainpage This documentation is probably unnecessary for this project.
 */

#include <Arduino.h>
#include <SD.h>

#include "SensorData.hpp"
#include "Sensors.hpp"
#include "TryInit.hpp"

extern "C" {
    uint16_t posit16_from_float (float f);
    float float_from_posit16 (uint16_t p);
}

SDFile file;
const uint8_t SD_PIN = 10;

void setup () {
    Serial.begin(9600);

    data.begin();

    TryInit(-1, SD, SD_PIN);

    file = SD.open("data.txt", FILE_WRITE);
}

const unsigned long dataReadDelay = 1000;
unsigned long last = 0;
unsigned long current;
unsigned long elapsed;
unsigned long total = 0;

void loop () {
    current = millis();
    elapsed = current - last;
    total += elapsed;
    
    if (elapsed > dataReadDelay) {
        data.update();
        data.write(file, total);
        data.debug();
        last = current;
    }
}