/*! \file SensorData.cpp */

#include <Arduino.h>
#include <SD.h>
#include <Adafruit_LTR390.h>

#include "SensorData.hpp"
#include "Sensors.hpp"

/*! \brief globally defined SensorData instance */
SensorData data;

/*!
 * \fn SensorData::begin
 * \brief Performs any necessary initialization
 */
void SensorData::begin () {
    while (!sensorBME.begin()) {
        Serial.println(F("failed to initialize BME sensor"));
        delay(1000);
    }
    Serial.println(F("initialized BME sensor"));

    while (!sensorOxygen.begin(ADDRESS_3)) {
        Serial.println(F("i2c device number error"));
        delay(1000);
    }
    Serial.println(F("initialized Oxygen sensor"));

    while (!sensorUV.begin()) {
        Serial.println(F("failed to initialize UV sensor"));
        delay(1000);
    }
    sensorUV.setMode(LTR390_MODE_UVS);
    sensorUV.setGain(LTR390_GAIN_3);
    // lower the resolution if reading are taking too long
    sensorUV.setResolution(LTR390_RESOLUTION_16BIT);
    sensorUV.setThresholds(0, 0);
    sensorUV.configInterrupt(false, LTR390_MODE_UVS);
    Serial.println(F("initialized UV sensor"));

    /*while (!sensorOrientation.begin()) {
        Serial.println(F("failed to initialize Orientation sensor"));
        delay(1000);
    }
    delay(1000);
    sensorOrientation.setExtCrystalUse(true);
    Serial.println(F("initialized Orientation sensor"));*/
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
    /*!
     * \warning Test whether the write function clears the file
     * as it does in linux/macos. If it does insert a seek before
     * the write
     */
    file.write((char *)this, sizeof(this));
}

void SensorData::debug () {
    Serial.print("temperature: ");
    Serial.println(temperature);
    Serial.print("humidity: ");
    Serial.println(humidity);
    Serial.print("pressure: ");
    Serial.println(pressure);
    Serial.print("UV: ");
    Serial.println(uv);
    Serial.print("oxygen: ");
    Serial.println(oxygen);
}