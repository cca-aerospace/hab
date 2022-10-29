/*!
 * \mainpage This documentation is probably unnecessary for this project.
 */

#include <Arduino.h>
#include <SD.h>

#include "SensorData.hpp"
#include "Sensors.hpp"
#include "TryInit.hpp"

SDFile file;
const uint8_t SD_PRIMARY_PIN = 10;
const uint8_t SD_SECONDARY_PIN = 9;

void setup() {
    Serial.begin(9600);

    while (!Serial) {
    }

    data.begin();

    TryInit(-1, SD, SD_PRIMARY_PIN);

    file = SD.open(F("data.txt"), FILE_WRITE | O_APPEND);

    data.primary = file;
}

const unsigned long dataReadDelay = 1000;
unsigned long last = 0;
unsigned long current;
unsigned long elapsed;
unsigned long total = 0;

void loop() {
    current = millis();
    elapsed = current - last;
    total += elapsed;

    if (elapsed > dataReadDelay) {
        data.update();
        data.write(total);
        data.debug();
        last = current;
    }

    if (digitalRead(2)) {
        while (1) {
        }
    }
}