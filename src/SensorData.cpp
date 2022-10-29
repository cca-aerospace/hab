/*! \file SensorData.cpp */

#include <Adafruit_LTR390.h>
#include <Arduino.h>
#include <SD.h>

#include "Debug.hpp"
#include "SensorData.hpp"
#include "Sensors.hpp"
#include "TryInit.hpp"

/*! \brief globally defined SensorData instance */
SensorData data;

/*!
 * \fn SensorData::begin
 * \brief Performs any necessary initialization
 */
void SensorData::begin() {
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
void SensorData::update() {
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

    accel = sensorOrientation.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);

    // update orientation
    orientation = sensorOrientation.getQuat();
}

/*!
 * \fn SensorData::write
 * \brief Writes data to file on SD card
 *
 * \param   file   The file to write the data into
 */
void SensorData::write(unsigned long ms) {
    struct Data {
        unsigned long ms;
        float temperature;
        float humidity;
        float pressure;
        float oxygen;
        float uv;
        float qx;
        float qy;
        float qz;
        float qw;
        float accelx;
        float accely;
        float accelz;
    };

    struct Data data;
    data.ms = ms;
    data.temperature = temperature;
    data.humidity = humidity;
    data.pressure = pressure;
    data.oxygen = oxygen;
    data.uv = uv;
    // unpack the structures to guarantee the order of elements
    // TODO: is this really necessary?
    data.qx = orientation.x();
    data.qy = orientation.y();
    data.qz = orientation.z();
    data.qw = orientation.w();
    data.accelx = accel.x();
    data.accely = accel.y();
    data.accelz = accel.z();

    int written = primary.write((uint8_t *)&data, sizeof(struct Data));

    // perform fallback strategy
    if (written != sizeof(struct Data)) {
        Println(F("FAIL"));
    } else {
        Println(F("SUCCESS"));
    }
}

void SensorData::debug() {
    Print(F("temperature: "));
    Println(temperature);
    Print(F("humidity: "));
    Println(humidity);
    Print(F("pressure: "));
    Println(pressure);
    Print(F("UV: "));
    Println(uv);
    Print(F("oxygen: "));
    Println(oxygen);
}