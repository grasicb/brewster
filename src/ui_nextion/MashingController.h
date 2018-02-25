#pragma once

#include "AWindowController.h"
#include "../controller/TemperatureSensor.h"
#include "../controller/SensorManager.h"
#include "../controller/Output.h"
#include "../process/MashingProcess.h"
#include "../process/Process.h"
#include <vector>
#include <map>

class MashingController : public AWindowController {

public:
  MashingController();
  void initializeScreen(void *ptr);
  void deactivateScreen();
  void process();

protected:


private:
  Logger *logger;

  //UI Components
  NexNumber nTempMT = NexNumber(6, 3, "n_mt");
  NexNumber nTempHLT = NexNumber(6, 4, "n_hlt");
  NexNumber nTempCOut = NexNumber(6, 5, "n_c_out");

  NexText tCurrStepTime = NexText(6, 7, "t_timelapse");
  NexNumber ncurrStepTemp = NexNumber(6, 13, "n_target_temp");
  NexText tNextStepTime = NexText(6, 15, "t_ns_time");
  NexNumber nNextStepTemp = NexNumber(6, 14, "n_ns_temp");

  NexProgressBar pbProgress = NexProgressBar(6, 6, "pb_progress");

  NexDSButton bPump = NexDSButton(6, 18, "bt_pump");
  NexButton bStartStop = NexButton(6, 16, "b0");

  String bStopText = String("STOP");
  String bStartText = String("START");

  //Temperature sensors
  std::map<SensorLocation, float> temperature;
  TempSensorMap* sensors;

  //Mash process information
  Process* mashProcess;
  Recipe* recipe;
  StepDO* currentStep;
  StepDO* nextStep;
  unsigned long processStartTime;
  unsigned long currentStepStartTime;
  unsigned long nextStepStartTime;
  unsigned long runTime;
  int remainingTime;
  uint8_t progress;

  //UI Refresh functions
  void updateLcdTemp();
  void updateLcdProcessInfo();
  void getRecipeInformation();
  void updateRecipeValues();

  //Callback functions
  static void triggerPumpButtonAH(void *ptr);
  static void triggerStartStopAH(void *ptr);
  static void processInfoChangeHandler(void* callingObject, void* process);
  static void processStateChangeHandler(void* callingObject, ProcessStateChangeEvent event);
  static void pumpStateChanged(void* callingObject, int outputIdentifier, OutputChangeEvent event);
};
