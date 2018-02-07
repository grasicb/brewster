#include "TemperatureSensor.h"

TemperatureSensor::TemperatureSensor(uint8_t sensorAddress[8], SensorLocation sensorLocation) {
    value = 0;
    location = sensorLocation;
    memcpy(address, sensorAddress, sizeof(address)); //address = sensorAddress;
}

void TemperatureSensor::readSensor() {
  if (active && millis() > lastRead + readInterval) {

    BrewsterGlobals::get()->i2cMutex.lock();
    DS18 *sensor = BrewsterController::get()->getDS18Interface();

    //Read processed sensor value (async reading of values)
    if (lastRead > 0 && sensor->asyncReadFetchData(address)) {
        value = sensor->celsius();

        if (Log.isTraceEnabled()) {
           logger->trace("Sensor %s: %.2f", (const char *)sensorNames[location], value);
        }
    }

    //request sensor refresh
    sensor->asyncReadRequest(address);
    lastRead = millis();
    BrewsterGlobals::get()->i2cMutex.unlock();
  }
}

float TemperatureSensor::getValue() {
    if (lastRead+valueValidity > millis()) {
      return value;
    }else{
      return 0;
    }
}

String TemperatureSensor::getAddressString() {
  return String::format("%2X  %2X %2X  %2X %2X  %2X %2X  %2X", address[0], address[1], address[2], address[3], address[4], address[5], address[6], address[7]);
}
void TemperatureSensor::setActive(boolean active) {
  this->active = active;
}

boolean TemperatureSensor::isActive() {
  return active;
}

TemperatureSensor::SensorLocation TemperatureSensor::getLocation() {
  return location;
}
