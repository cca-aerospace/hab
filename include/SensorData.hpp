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

        // TODO: determine smallest width needed to hold data

        /*! \brief Celsius */
        int temperature;
        /*! \brief Relative humidity */
        int humidity;
        /*! \brief Pascals */
        int pressure;
        /*! \brief Percent concentration */
        int oxygen;
        /*! \brief UV index */
        int uv;
        /*! \brief Quaternion */
        imu::Quaternion orientation;

        void begin ();

        void update ();

        void write (SDFile file);
};

#endif