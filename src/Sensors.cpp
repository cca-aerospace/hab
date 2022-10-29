/*! \file Sensors.cpp */
#include "Sensors.hpp"

/*! \brief Sensor to check orientation */
Adafruit_BNO055 sensorOrientation = Adafruit_BNO055(55);

/*! \brief Sensor to read UV index */
Adafruit_LTR390 sensorUV = Adafruit_LTR390();

/*! \brief Sensor to read oxygen concentration */
DFRobot_OxygenSensor sensorOxygen;

/*! \brief Sensor to read pressure, humidity, and temperature */
Adafruit_BME280 sensorBME;