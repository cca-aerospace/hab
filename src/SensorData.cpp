/*! \file SensorData.cpp */

#include <Arduino.h>
#include <SD.h>
#include <Adafruit_LTR390.h>

#include "SensorData.hpp"
#include "Sensors.hpp"

/*!
 * \fn SensorData::begin
 * \brief Performs any necessary initialization
 */
void SensorData::begin () {
    while (!sensorBME.begin()) {
        Serial.println("failed to initialize BME sensor");
    }
    Serial.println("initialized BME sensor");

    while (!sensorOxygen.begin()) {
        Serial.println("i2c device number error");
    }
    Serial.println("initialized Oxygen sensor");

    while (!sensorUV.begin()) {
        Serial.println("failed to initialize UV sensor");
    }
    sensorUV.setMode(LTR390_MODE_UVS);
    sensorUV.setGain(LTR390_GAIN_3);
    // lower the resolution if reading are taking too long
    sensorUV.setResolution(LTR390_RESOLUTION_16BIT);
    sensorUV.setThresholds(0, 0);
    sensorUV.configInterrupt(false, LTR390_MODE_UVS);
    Serial.println("initialized UV sensor");

    while (!sensorOrientation.begin()) {
        Serial.println("failed to initialize Orientation sensor");
    }
    delay(1000);
    sensorOrientation.setExtCrystalUse(true);
    Serial.println("initialized Orientation sensor");
}

/*!
 * \fn SensorData::update
 * \brief Reads data from sensors
 */
void SensorData::update () {
    // pull data from sensors

    // update temp, humidity, and pressure
    temperature = sensorBME.readTemperature();
    humidity = sensorBME.readHumidity();
    pressure = sensorBME.readPressure();

    // update oxygen
    oxygen = sensorOxygen.getOxygenData(5);
    
    // update uv
    if (sensorUV.newDataAvailable()) {
        uv = sensorUV.readUVS();
    }

    // update orientation
    orientation = sensorOrientation.getQuat();
}

/*!
 * \fn SensorData::write
 * \brief Writes data to file on SD card
 * 
 * \param   file   The file to write the data into
 */
void SensorData::write (SDFile file) {

}