#pragma once

#include "application.h"
#include "TemperatureSensor.h"
#include <map>

class SensorManager {
public:
    SensorManager();
    ~SensorManager() {};
    void readSensors();
    void readTemperatureSensors();
    TemperatureSensor& getTemperatureSensor(SensorLocation sensorLocation);
    std::map<SensorLocation, TemperatureSensor>& getAllTemperatureSensors();
    uint8_t getTemperatureSensorNumber();

private:
  Logger *logger;
  std::map<SensorLocation, TemperatureSensor> temperatureSensors;
};
