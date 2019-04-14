#pragma once

#include "application.h"
#include "../util/BrewsterGlobals.h"
#include "TemperatureSensor.h"
#include "../lib/cloud_connect/CloudConnect.h"
#include <map>

class SensorManager {
public:
    typedef std::map<SensorLocation, TemperatureSensor> TempSensorMap;

    SensorManager();
    ~SensorManager() {};
    void readSensors();
    void readTemperatureSensors();
    TemperatureSensor& getTemperatureSensor(SensorLocation sensorLocation);
    TempSensorMap& getAllTemperatureSensors();
    uint8_t getTemperatureSensorNumber();

private:
  Logger *logger;
  TempSensorMap temperatureSensors;
};
