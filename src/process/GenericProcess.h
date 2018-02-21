#pragma once

#include "application.h"
#include "Process.h"

class GenericProcess : public Process {
public:
  GenericProcess(BrewProcess type, String name);
  void process();
private:
  //Logger *logger;
  unsigned long lastTick;
};
