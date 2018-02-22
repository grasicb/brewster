#pragma once

#include "application.h"
#include "../util/BrewsterGlobals.h"
#include "TemperatureSensor.h"
#include <map>

typedef std::map<SensorLocation, TemperatureSensor> TempSensorMap;

class SensorManager {
public:
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
