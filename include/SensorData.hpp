/*! \file SensorData.hpp
 *
 */

#ifndef __SENSOR_DATA_HPP_
#define __SENSOR_DATA_HPP_

/*! \class SensorData
 *  \brief Class to handle reading data from sensors
 *
 */
class SensorData {
    private:

    public:

        // TODO: determine smallest width needed to hold data
        int temperature;
        int humidity;
        int pressure;
        int oxygen;
        int uv;

        void begin ();

        void update ();
};

#endif