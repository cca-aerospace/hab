/*!
 * \mainpage This documentation is probably unnecessary for this project.
 */

#include <Arduino.h>
#include <SD.h>

#include "Debug.hpp"
#include "SensorData.hpp"
#include "Sensors.hpp"
#include "Time.hpp"
#include "TryInit.hpp"

void setup() {
#ifdef MODE_DEBUG
    Serial.begin(9600);

    while (!Serial) {
    }
#endif
    setPinMode(A0, INPUT);

    data.begin();
}

const time_t dataReadDelay = seconds(1);

#ifdef MODE_DEBUG
const time_t shutdownTime = minutes(5);
#else
const time_t shutdownTime = hours(24);
#endif

time_t last = 0;
time_t elapsed;

void loop() {
    data.current = millis();
    elapsed = data.current - last;

    if (elapsed > dataReadDelay) {
        data.update();
        // the millis function will overflow after approx 49.7 days
        // this is more than enough time
        data.write();
        last = data.current;
    }

    if (data.current > shutdownTime) {
        if (data.file) {
            data.file.close();
        }
        Println(F("shut"));
        while (1) {
        }
    }
}