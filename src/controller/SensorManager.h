#pragma once

#include "application.h"
#include "TemperatureSensor.h"


class SensorManager {
public:
    SensorManager();
    ~SensorManager() {};
    void readSensors();
    void readTemperatureSensors();
    TemperatureSensor *getTemperatureSensor(TemperatureSensor::SensorLocation sensorLocation);
    TemperatureSensor **getAllTemperatureSensors();
    uint8_t getTemperatureSensorNumber();

private:
  Logger *logger;
  TemperatureSensor **temperatureSensors;
  uint8_t tempSensorsNo;
};
