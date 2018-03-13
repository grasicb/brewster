#pragma once

#include "application.h"
#include "Process.h"

class BoilingProcess : public Process {
public:
  BoilingProcess(BrewProcess type, String name);
  BoilingProcess(BrewProcess type, String name, Recipe* recipe);
  void process();
  boolean isBoilingPointReached();

protected:
  void processStarted();
  void processStopped();
  void processPaused();
  void processResumed();
  void processRestored();
private:
  void updateOutput();
  void updateStepStatus();

  boolean boilingPointReached;
  boolean boilingAlarm;
  boolean nextAdditionAlarm1;
  boolean nextAdditionAlarm2;
  unsigned long alarm1 = 180;
  unsigned long alarm2 = 60;
  long boilStopTime;
  long nextAdditionTime;

  unsigned long lastTick;
};
