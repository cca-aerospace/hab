/*! \file SensorData.cpp */

#include <Arduino.h>
#include <SD.h>

#include "SensorData.hpp"
#include "Sensors.hpp"

/*!
 * \fn SensorData::begin
 * \brief Performs any necessary initialization
 */
void SensorData::begin () {

}

/*!
 * \fn SensorData::update
 * \brief Reads data from sensors
 */
void SensorData::update () {
    // pull data from sensors

    // update temp, humidity, and pressure

    // update oxygen
    
    // update uv

    // update orientation
}

/*!
 * \fn SensorData::write
 * \brief Writes data to file on SD card
 * 
 * \param   file   The file to write the data into
 */
void SensorData::write (SDFile file) {

}