#include <Adafruit_SGP30.h>
#include <Arduino.h>
#include <DFRobot_OzoneSensor.h>
#include <Wire.h>

/*
 *   Wiring up the arduino mini
 *   RX to RX
 *   TX to TX
 *   DTR to RST
 *   VCC to RAW
 *   GND to GND
 */

#include <Adafruit_SGP30.h>
#include <Arduino.h>
#include <DFRobot_OzoneSensor.h>
#include <Wire.h>

Adafruit_SGP30 sgp;

#define COLLECT_NUMBER 100 // collect number, the collection range is 1-100
/**
 * select i2c device address
 *   OZONE_ADDRESS_0  0x70
 *   OZONE_ADDRESS_1  0x71
 *   OZONE_ADDRESS_2  0x72
 *   OZONE_ADDRESS_3  0x73
 */
#define Ozone_IICAddress OZONE_ADDRESS_3
DFRobot_OzoneSensor Ozone;

void setup() {
    Serial.begin(9600);

    while (!Ozone.begin(Ozone_IICAddress)) {
        Serial.println("Ozone I2c device number error !");
        delay(1000);
    }
    Serial.println("Ozone I2c connect success !");

    /**
     * set measuer mode
     * MEASURE_MODE_AUTOMATIC         active  mode
     * MEASURE_MODE_PASSIVE           passive mode
     */
    Ozone.setModes(MEASURE_MODE_PASSIVE);

    Serial.println("Testing, testing");

    if (!sgp.begin()){
        Serial.println("SGP30 Sensor not found");
        while (1);
    }
    Serial.print("Found SGP30 serial #");
    Serial.print(sgp.serialnumber[0], HEX);
    Serial.print(sgp.serialnumber[1], HEX);
    Serial.println(sgp.serialnumber[2], HEX);

    while (1) {}
}

void loop() {
    /**
     * Smooth data collection
     * COLLECT_NUMBER                 The collection range is 1-100
     */
    int16_t ozoneConcentration = Ozone.readOzoneData(COLLECT_NUMBER);
    Serial.print("Ozone concentration is ");
    Serial.print(ozoneConcentration);
    Serial.println(" PPB.");
    delay(1000);
}