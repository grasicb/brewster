#pragma once

#include "AWindowController.h"
#include "../controller/TemperatureSensor.h"
#include "../controller/SensorManager.h"
#include "../controller/Output.h"
#include "../process/Process.h"

class CoolingWC : public AWindowController {

public:
  CoolingWC();
  void initializeScreen(void *ptr);
  void deactivateScreen();
  void process();

protected:


private:
  Logger *logger;

  //UI Elements
  NexNumber nTargetTemp = NexNumber(8, 6, "n_targetTemp");
  NexText tTimeElapsed = NexText(8, 3, "t_timelapse");
  NexText tTempBK = NexText(8, 8, "t_tempBK");
  NexText tTempCooler = NexText(8, 9, "t_tempCOut");
  NexText tTempFermentor = NexText(8, 15, "t_tempFerm");

  NexButton b1 = NexButton(8, 10, "b1");
  NexButton b2 = NexButton(8, 11, "b2");
  NexDSButton bSettings = NexDSButton(8, 14, "bt1");
  NexDSButton bPump = NexDSButton(8, 7, "bt_pump");

  String bStopText = String("STOP");
  String bStartText = String("START");
  String bPauseText = String("PAUSE");
  String bResumeText = String("RESUME");

  //Boil process information
  Process *coolingProcess;
  Recipe *recipe;
  TemperatureSensor *tempSensorBK;
  TemperatureSensor *tempSensorCooler;
  TemperatureSensor *tempSensorFermentor;
  float lastTempBK = 0;
  float lastTempCooler = 0;
  float lastTempFermentor = 0;

  long runTime = 0;
  unsigned long startTime = 0;

  //UI Refresh functions
  void updateTime();
  void updateButtonText();
  void setStartTime();

  //Callback functions
  static void bTriggerProcessCB(void *ptr);
  static void triggerPumpButtonAH(void *ptr);
  static void triggerSettingsButtonAH(void *ptr);
  static void processStateChangeHandler(void* callingObject, ProcessStateChangeEvent event);
  static void pumpStateChanged(void* callingObject, int outputIdentifier, Output::OutputChangeEvent event);
};
