#pragma once

#include "AWindowController.h"
#include "../controller/TemperatureSensor.h"
#include <vector>
#include <map>

class SensorTestController : public AWindowController {

public:
  SensorTestController();
  void initializeScreen(void *ptr);
  void process();

protected:


private:
  Logger *logger;
  NexText outputText1 = NexText(4, 1, "t0");
  NexText outputText2 = NexText(4, 3, "t1");
  std::vector<float> temperature;
  std::map<SensorLocation, TemperatureSensor>* sensors;
  uint8_t sensorNo;

  void updateOutputText();
};
