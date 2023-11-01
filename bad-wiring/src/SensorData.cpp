/*! \file SensorData.cpp */

#include <Adafruit_LTR390.h>
#include <Arduino.h>
#include <SD.h>

#include "Debug.hpp"
#include "SensorData.hpp"
#include "Sensors.hpp"
#include "Time.hpp"
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

    failed = true;
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
    altitude = sensorBME.readAltitude(1013.25);

    // Print(F("temp: ")); Println(temperature);

    // update oxygen
    oxygen = sensorOxygen.getOxygenData(5);

    uv = sensorUV.readUVS();

    // Print(F("uv: ")); Println(uv);

    // update orientation
    auto orientation =
        sensorOrientation.getVector(Adafruit_BNO055::VECTOR_EULER);
    eulerx = orientation.x();
    eulery = orientation.y();
    eulerz = orientation.z();

    // update acceleration
    auto accel =
        sensorOrientation.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    accelx = accel.x();
    accely = accel.y() + 0.275;
    accelz = accel.z() + 0.3475;

    voltage = analogRead(A0);
}

const uint8_t PRIMARY_SD = 10;

void SensorData::write() {
    failed = _write(PRIMARY_SD);
    if (!failed) {
        file.flush();
    }
}

bool SensorData::open(uint8_t pin) {
    auto fname = F("DATA.TXT");
    bool status = SD.begin(pin) && (file = SD.open(fname, O_READ | O_WRITE | O_CREAT));
    if (status) {
        file.seek(0);
    }
    return status;
}

/*!
 * \fn SensorData::write
 * \brief Writes data to file on SD card
 *
 * \param   file   The file to write the data into
 */
bool SensorData::_write(uint8_t pin) {
    struct Data {
        time_t ms;
        float temperature;
        float humidity;
        float altitude;
        float oxygen;
        float uv;
        float ex;
        float ey;
        float ez;
        float accelx;
        float accely;
        float accelz;
        short voltage;
    };

    if (failed && (!open(pin))) {
        return false;
    }

    uint8_t attempts = 0;
    uint8_t written = 0;
    failed = true;

    while (failed && attempts < 4) {
        written = file.write((uint8_t *)&data.current, sizeof(struct Data));
        failed = written != sizeof(struct Data);
        if (failed) {
            attempts++;
        } else {
            break;
        }
    }

    Println(written);
    return failed;
}