#pragma once

#include "AWindowController.h"
#include "../controller/TemperatureSensor.h"
#include <map>

class OutputTestController : public AWindowController {

public:
  OutputTestController();
  void initializeScreen(void *ptr);
  void process();

protected:


private:
  Logger *logger;
  float temperature;
  std::map<SensorLocation, TemperatureSensor>* sensors;
  std::map<SensorLocation, TemperatureSensor>::iterator it;
  SensorLocation selectedSensor;
};
