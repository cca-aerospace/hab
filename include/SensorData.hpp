/*! \file SensorData.hpp */

#ifndef __SENSOR_DATA_HPP_
#define __SENSOR_DATA_HPP_

#include <SD.h>

/*!
 * \class SensorData
 * \brief Class to handle reading data from sensors
 */
class SensorData {
    private:

    public:

        // TODO: determine smallest width needed to hold data

        int temperature; /*! \brief Celsius */
        int humidity;    /*! \brief Relative humidity */
        int pressure;    /*! \brief Pascals */
        int oxygen;      /*! \brief Percent concentration */
        int uv;          /*! \brief UV index */

        void begin ();

        void update ();

        void write (SDFile file);
};

#endif