#pragma once

#include "AWindowController.h"
#include "../controller/TemperatureSensor.h"
#include "../controller/SensorManager.h"
#include "../controller/Output.h"

class WaterPreparationWC : public AWindowController {

public:
  WaterPreparationWC();
  void initializeScreen(void *ptr);
  void deactivateScreen();
  void process();

protected:


private:
  Logger *logger;

  //UI Elements
  NexText tTempHLT = NexText(11, 15, "t_tempHLT");
  NexText tTempBK = NexText(11, 16, "t_tempBK");
  NexDSButton bHLTPower = NexDSButton(11, 13, "bt3");
  NexDSButton bHLTSettings = NexDSButton(11, 9, "bt1");
  NexDSButton bBKPower = NexDSButton(11, 14, "bt4");
  NexDSButton bBKSettings = NexDSButton(11, 10, "bt2");
  NexNumber nTargetHLT = NexNumber(11, 11, "nTargetHLT");
  NexNumber nTargetBK = NexNumber(11, 12, "nTargetBK");

  //Sreen relevant data
  TemperatureSensor *tempSensorHLT;
  TemperatureSensor *tempSensorBK;
  Output *outputHLT;
  Output *outputBK;
  float lastTempHLT = 0;
  float lastTempBK = 0;

  //Sreen related functions
  void refreshOutputStatus();

  //Callback functions
  static void bTriggerPowerCB(void *ptr);
  static void bTriggerSettingsCB(void *ptr);
  static void outputChangedEvent(void* callingObject, int outputIdentifier, Output::OutputChangeEvent event);
};
