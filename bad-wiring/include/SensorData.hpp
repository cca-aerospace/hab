/*! \file SensorData.hpp */

#ifndef __SENSOR_DATA_HPP_
#define __SENSOR_DATA_HPP_

#include "Time.hpp"
#include <SD.h>

/*!
 * \class SensorData
 * \brief Class to handle reading data from sensors
 */
class SensorData {
private:
    bool _write(uint8_t pin);

public:
    SDFile file;
    bool failed;

    time_t current;
    /*! \brief Celsius */
    float temperature;
    /*! \brief Relative humidity */
    float humidity;
    /*! \brief Pascals */
    float altitude;
    /*! \brief Percent concentration */
    float oxygen;
    /*! \brief UV index */
    float uv;
    /*! \brief Euler angles */
    float eulerx;
    float eulery;
    float eulerz;
    /*! \brief Acceleration */
    float accelx;
    float accely;
    float accelz;
    short voltage;

    void begin();

    void update();

    bool open(uint8_t pin);

    void write();
};

extern SensorData data;

#endif