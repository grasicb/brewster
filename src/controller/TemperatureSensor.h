#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include "application.h"

#include "../util/BrewsterGlobals.h"
#include "BrewsterController.h"


class TemperatureSensor {
public:
    enum SensorLocation {HLT, MASH_TUN, BOIL_KETTLE, COOLER_IN, COOLER_OUT, FERMENTOR, FRIDGE, ROOM};
    static String sensorNames[8];

    TemperatureSensor(uint8_t sensorAddress[8], SensorLocation sensorLocation);
    ~TemperatureSensor() { delete address;};
    void readSensor();

    float getValue();
    String getAddressString();
    void setActive(boolean active);
    boolean isActive();
    SensorLocation getLocation();


private:
  Logger *logger;
  float value;
  uint8_t address[8];
  boolean active; //For future use - currently always true
  unsigned long lastRead;
  SensorLocation location;

  //Time out variables
  const static unsigned long valueValidity = 300000; //value read from sensor is valid for this value (5min), after this timeout value 0 is returned
  const static unsigned long readInterval = 400; //Value is requested from sensor every 400 ms - this is the time DS18 needs for processing 11 bit precision value
};

String TemperatureSensor::sensorNames[] = {"Hot liquor tank", "Mash tun", "Boil kettle", "Cooler input", "Coller output", "Fermentor", "Fridge", "Room"};
#endif // TEMPERATURE_SENSOR_H
