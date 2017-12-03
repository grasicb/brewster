#ifndef TEMPERATURE_SENSORS_H
#define TEMPERATURE_SENSORS_H

#include "application.h"
#include <forward_list>
#include "../lib/sensors/DS18.h"
#include "../util/BrewsterGlobals.h"


class TemperatureSensors {
public:
    TemperatureSensors();
    ~TemperatureSensors() {};
    void readSensors();

    float temperature[10];

private:
  std::forward_list<uint8_t> activeSensors;
  DS18 *ds18Sensor;
  Logger *logger;
  long lastRead;
};


#endif // TEMPERATURE_SENSORS_H
