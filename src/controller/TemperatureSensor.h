#pragma once
#include "application.h"
#include "../util/BrewsterGlobals.h"

class TemperatureSensor {
public:
    TemperatureSensor();
    TemperatureSensor(uint8_t sensorAddress[8], SensorLocation sensorLocation);
    ~TemperatureSensor() { };
    void readSensor();

    float getValue();
    float* getValueReference();
    String getAddressString();
    void setActive(boolean active);
    boolean isActive();
    SensorLocation getLocation();
    void sendTempChangeEvent();


private:
  Logger *logger;
  float value;
  float lastValue = 0;
  uint8_t address[8];
  boolean active; //For future use - currently always true
  unsigned long lastRead;
  SensorLocation location;

  //Time out variables
  const static unsigned long valueValidity = 60000; //value read from sensor is valid for this value (1min), after this timeout value 0 is returned
  const static unsigned long readInterval = 400; //Value is requested from sensor every 400 ms - this is the time DS18 needs for processing 11 bit precision value
};

//String TemperatureSensor::sensorNames[] = {"Hot liquor tank", "Mash tun", "Boil kettle", "Cooler input", "Coller output", "Fermentor", "Fridge", "Room"};
