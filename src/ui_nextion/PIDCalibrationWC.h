#pragma once

#include "AWindowController.h"
#include <vector>

class PIDCalibrationWC : public AWindowController {

public:
  PIDCalibrationWC();
  void initializeScreen(void *ptr);
  void deactivateScreen();
  void process();

protected:


private:
  Logger *logger;

  //UI Elements
  NexText tOutput = NexText(12, 1, "t0");

  NexButton bHLT = NexButton(12, 4, "b1");
  NexButton bBK = NexButton(12, 5, "b2");
  NexButton bMT = NexButton(12, 6, "b3");
  NexButton bCooling = NexButton(12, 7, "b4");
  NexButton bFermentation = NexButton(12, 8, "b5");


  //Boil process information
  std::vector<String> output;
  PID *pid;
  PID_ATune *aTune;
  double kp;
  double ki;
  double kd;

  //UI Refresh functions
  void addOutput(String text);
  void refreshOutput();

  //Callback functions
  static void bTriggerCalibrationButtonCB(void *ptr);
};
