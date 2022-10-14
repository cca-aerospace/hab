/*! \file SensorData.cpp */

#include <Arduino.h>
#include <SD.h>
#include <Adafruit_LTR390.h>

#include "SensorData.hpp"
#include "Sensors.hpp"
#include "TryInit.hpp"

/*! \brief globally defined SensorData instance */
SensorData data;

/*!
 * \fn SensorData::begin
 * \brief Performs any necessary initialization
 */
void SensorData::begin () {
    TryInit(10, sensorBME);

    TryInit(10, sensorOxygen, ADDRESS_3);

    TryInit(10, sensorUV);
    sensorUV.setMode(LTR390_MODE_UVS);
    sensorUV.setGain(LTR390_GAIN_3);
    // lower the resolution if reading are taking too long
    sensorUV.setResolution(LTR390_RESOLUTION_16BIT);
    sensorUV.setThresholds(0, 0);
    sensorUV.configInterrupt(false, LTR390_MODE_UVS);

    TryInit(10, sensorOrientation);
    sensorOrientation.setExtCrystalUse(true);
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
void SensorData::write (SDFile file, unsigned long ms) {
    int written = 0;

    struct Data {
        unsigned long ms;
        float temperature;
        float humidity;
        float pressure;
        float oxygen;
        float uv;
    };

    struct Data * data = new Data;
    data->ms = ms;
    data->temperature = temperature;
    data->humidity = humidity;
    data->pressure = pressure;
    data->oxygen = oxygen;
    data->uv = uv;

    written += file.write((char *)data, sizeof(struct Data));

    Serial.print("bytes written: ");
    Serial.println(written);

    delete data;
}

void SensorData::debug () {
    Serial.print(F("temperature: "));
    Serial.println(temperature);
    Serial.print(F("humidity: "));
    Serial.println(humidity);
    Serial.print(F("pressure: "));
    Serial.println(pressure);
    Serial.print(F("UV: "));
    Serial.println(uv);
    Serial.print(F("oxygen: "));
    Serial.println(oxygen);
}