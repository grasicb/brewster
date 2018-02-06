#pragma once

#include "AWindowController.h"

class SensorSearchController : public AWindowController {

public:
  SensorSearchController();
  void initializeScreen(void *ptr);
  void process();

protected:


private:
  Logger *logger;
  NexText outputText = NexText(3, 1, "t0");
  float temperature[10];

  void updateOutputText();
};
