/*!
 * \mainpage This documentation is probably unnecessary for this project.
 */

#include <Arduino.h>
#include <SD.h>

#include "SensorData.hpp"
#include "Sensors.hpp"
#include "TryInit.hpp"

extern "C" {
    float posit16_to_float (uint16_t p);
    uint64_t posit16_to_triple (uint16_t p);

    uint16_t posit16_from_float (float f);
    uint16_t posit16_from_triple (uint16_t fraction, uint8_t scale, uint8_t sign);
}

SDFile file;
const uint8_t SD_PIN = 10;

void setup () {
    Serial.begin(9600);

    while (!Serial) {}

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