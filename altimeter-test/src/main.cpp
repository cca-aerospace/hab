/*!
 * \mainpage This documentation is probably unnecessary for this project.
 */

#include <Arduino.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

void setup() {
    Serial.begin(9600);

    while (!Serial) {
    }

    while (!bme.begin()) {
        Serial.println("cannot find BME");
    }
}

void loop() {
    float humidity = bme.readHumidity();
    float pressure = bme.readPressure();
    float temperature = bme.readTemperature();

    Serial.print("humidity: ");
    Serial.println(humidity);
    Serial.print("pressure: ");
    Serial.println(pressure);
    Serial.print("temperature: ");
    Serial.println(temperature);
}