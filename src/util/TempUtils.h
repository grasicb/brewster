#ifndef TEMP_UTILS_H
#define TEMP_UTILS_H

#include "../lib/sensors/OneWire.h"
#include "../lib/sensors/DS18.h"
#include "BrewsterGlobals.h"

class TempUtils {
public:
  static void listSensors();
  static void setPrecision(DS18::PRECISION p);
  static void readTemperature();
};

#endif // TEMP_UTILS_H
