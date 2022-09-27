#ifndef __SENSOR_DATA_HPP_
#define __SENSOR_DATA_HPP_

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