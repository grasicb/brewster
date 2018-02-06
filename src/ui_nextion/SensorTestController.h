#pragma once

#include "AWindowController.h"

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
  float temperature[10];

  void updateOutputText();
};
