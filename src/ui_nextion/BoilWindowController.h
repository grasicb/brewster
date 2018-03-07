#pragma once

#include "AWindowController.h"
#include "../controller/TemperatureSensor.h"
#include "../controller/SensorManager.h"
#include "../controller/Output.h"
#include "../process/BoilProcess.h"

class BoilWindowController : public AWindowController {

public:
  BoilWindowController();
  void initializeScreen(void *ptr);
  void deactivateScreen();
  void process();

protected:


private:
  Logger *logger;

  //UI Elements
  NexText tAdd[6] = {
      NexText(7, 12, "t_Add1"),
      NexText(7, 13, "t_Add2"),
      NexText(7, 14, "t_Add3"),
      NexText(7, 15, "t_Add4"),
      NexText(7, 16, "t_Add5"),
      NexText(7, 17, "t_Add6")
    };
  NexText tTimeElapsed = NexText(7, 7, "t_timeElapsed");
  NexText tTimeRemaining = NexText(7, 9, "t_timeRemain");
  NexText tTemp = NexText(7, 19, "t_temp");
  NexProgressBar pbProgress = NexProgressBar(7, 5, "pb_progress");
  NexButton b1 = NexButton(7, 3, "b1");
  NexButton b2 = NexButton(7, 18, "b2");

  String bStopText = String("STOP");
  String bStartText = String("START");
  String bPauseText = String("PAUSE");
  String bResumeText = String("RESUME");

  //Boil process information
  BoilProcess *boilProcess;
  Recipe *recipe;
  TemperatureSensor *tempSensor;
  float lastTemp = 0;
  long totalTime = 0;
  long runTime = 0;
  long startTime = 0;

  //UI Refresh functions
  void updateAdditions();
  void updateTime();
  void updateButtonText();
  void setStartTime();

  //Callback functions
  static void bTriggerProcessCB(void *ptr);
  static void processInfoChangeHandler(void* callingObject, void* process);
  static void processStateChangeHandler(void* callingObject, ProcessStateChangeEvent event);
};
