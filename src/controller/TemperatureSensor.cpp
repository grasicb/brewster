#include "TemperatureSensor.h"
#include "BrewsterController.h"
#include "../util/BrewsterGlobals.h"

#include "../lib/cloud_connect/CloudConnect.h"
#include <ArduinoJson.h>

TemperatureSensor::TemperatureSensor() {
  logger = new Logger("temp_sensor");
  value = 0;
  lastValue = 0;
  active = false;
  lastRead = 0;
}

TemperatureSensor::TemperatureSensor(uint8_t sensorAddress[8], SensorLocation sensorLocation) {
    logger = new Logger("temp_sensor");
    value = 0;
    lastValue = 0;
    location = sensorLocation;
    active = true;
    lastRead = 0;
    memcpy(address, sensorAddress, sizeof(address)); //address = sensorAddress;
}

void TemperatureSensor::readSensor() {
  if (active && millis() > lastRead + readInterval) {

    BrewsterGlobals::get()->i2cMutex.lock();
    DS18 *sensor = BrewsterController::get()->getDS18Interface();

    //Read processed sensor value (async reading of values)
    if (lastRead > 0 && sensor->asyncReadFetchData(address)) {
        value = sensor->celsius();

        if(value != lastValue) {
          sendTempChangeEvent();          
          lastValue = value;
        }
/*
        if (Log.isTraceEnabled()) {
           logger->trace("Sensor %s: %.2f - Ref: 0x%X", (const char *)sensorNames[location], getValue(),  this);
        }
//*/
    }

    //request sensor refresh
    sensor->asyncReadRequest(address);
    lastRead = millis();
    BrewsterGlobals::get()->i2cMutex.unlock();
  }
}

float TemperatureSensor::getValue() {
    if (millis() - lastRead < valueValidity) {
      return value;
    }else{
      return 0;
    }
}

float *TemperatureSensor::getValueReference() {
    return &value;
}

String TemperatureSensor::getAddressString() {
  return String::format("%2X %2X %2X %2X %2X %2X %2X %2X", address[0], address[1], address[2], address[3], address[4], address[5], address[6], address[7]);
}
void TemperatureSensor::setActive(boolean active) {
  this->active = active;
}

boolean TemperatureSensor::isActive() {
  return active;
}

SensorLocation TemperatureSensor::getLocation() {
  return location;
}

void TemperatureSensor::sendTempChangeEvent() {
  ulong ttime = Time.now();
  const int capacity = JSON_OBJECT_SIZE(8+1);
  StaticJsonBuffer<capacity> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["type"] = "event";
  root["event"] = "temperatureChange";
  root["timestamp"] = ttime;
  std::string str (Time.format(ttime, TIME_FORMAT_ISO8601_FULL).c_str());
  root["timestamp_human"] = str;
  JsonObject& payload = root.createNestedObject("payload");
  payload["value"] = value;
  std::string sId (sensorShortNames[location] .c_str());
  payload["id"] = sId;

  CloudConnect* cc = BrewsterController::get()->getCloudConnectInstance();
  cc->emitEvent(root);
}