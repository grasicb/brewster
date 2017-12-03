#include "TemperatureSensors.h"

TemperatureSensors::TemperatureSensors() {
    logger = new Logger("temp_sensor");
    activeSensors.push_front(0);
    activeSensors.push_front(1);
    activeSensors.push_front(2);
    activeSensors.push_front(3);

    lastRead = 0;

    ds18Sensor = new DS18(BrewsterGlobals::get()->pinOneWire);
}


void TemperatureSensors::readSensors() {
  if (millis() > lastRead + 200) {

    BrewsterGlobals::get()->i2cMutex.lock();
    for(uint8_t i: activeSensors) {
      //fetch results from previous refresh
      if (lastRead != 0 && ds18Sensor->asyncReadFetchData(BrewsterGlobals::get()->tempSensors[i])) {
        temperature[i] = ds18Sensor->celsius();

        //if (Log.isTraceEnabled()) {
        //   logger->trace("Sensor %d: %.2f", i, ds18Sensor->celsius());
        //}
      }
      //request sensor refresh
      ds18Sensor->asyncReadRequest(BrewsterGlobals::get()->tempSensors[i]);
    }
    BrewsterGlobals::get()->i2cMutex.unlock();

    lastRead = millis();
  }
}
