/*! \file Sensors.hpp */

#ifndef __SENSORS_HPP_
#define __SENSORS_HPP_

#include <Adafruit_BME280.h>
#include <Adafruit_BNO055.h>
#include <Adafruit_LTR390.h>
#include <DFRobot_OxygenSensor.h>

extern Adafruit_BNO055 sensorOrientation;

extern Adafruit_LTR390 sensorUV;

extern DFRobot_OxygenSensor sensorOxygen;

extern Adafruit_BME280 sensorBME;

#endif