/*! \file SensorData.hpp */

#ifndef __SENSOR_DATA_HPP_
#define __SENSOR_DATA_HPP_

#include <SD.h>
#include <utility/imumaths.h>

/*!
 * \class SensorData
 * \brief Class to handle reading data from sensors
 */
class SensorData {
private:
public:
    SDFile primary;
    SDFile secondary;

    // TODO: determine smallest width needed to hold data

    /*! \brief Celsius */
    float temperature;
    /*! \brief Relative humidity */
    float humidity;
    /*! \brief Pascals */
    float pressure;
    /*! \brief Percent concentration */
    float oxygen;
    /*! \brief UV index */
    float uv;
    /*! \brief Quaternion */
    imu::Quaternion orientation;

    imu::Vector<3> accel;

    void begin();

    void update();

    void write(unsigned long ms);

    void fallback(unsigned long ms);

    void debug();
};

extern SensorData data;

#endif