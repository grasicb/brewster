#pragma once

#include "AWindowController.h"
#include "../controller/TemperatureSensor.h"
#include "../controller/Output.h"
#include <map>

class OutputTestController : public AWindowController {

public:
  //Inherited functions
  OutputTestController();
  void initializeScreen(void *ptr);
  void deactivateScreen();
  void process();


  //Business functions
  void refreshScreen();
  void updateTempSensorLocation();
  void updateTempSensorValue();
  void nextSensor();

protected:


private:
  //Global variables needed for the logic
  Logger *logger;
  float temperature;
  std::map<SensorLocation, TemperatureSensor>* sensors;
  std::map<SensorLocation, TemperatureSensor>::iterator it;
  SensorLocation selectedSensor;


  //UI Components
  NexDSButton bPowerSwitches[4] = {
              NexDSButton(5, 11, "bt1"),
              NexDSButton(5, 12, "bt2"),
              NexDSButton(5, 13, "bt3"),
              NexDSButton(5, 14, "bt4")};

  NexDSButton bAutoSwitches[4] = {
              NexDSButton(5, 15, "bt5"),
              NexDSButton(5, 16, "bt6"),
              NexDSButton(5, 17, "bt7"),
              NexDSButton(5, 18, "bt8")};

  NexDSButton bSettings[4] = {
              NexDSButton(5, 23, "bt9"),
              NexDSButton(5, 24, "bt10"),
              NexDSButton(5, 25, "bt11"),
              NexDSButton(5, 26, "bt12")};

  NexNumber nValues[4] = {
              NexNumber(5, 27, "n1"),
              NexNumber(5, 28, "n2"),
              NexNumber(5, 29, "n3"),
              NexNumber(5, 30, "n4")};

  NexNumber nTemperature = NexNumber(5, 31, "n_temp");
  NexText tLocation = NexText(5, 33, "t_temp_loc");
  NexButton bNextLocation = NexButton(5, 34, "b_next_sensor");
  NexVar vPreviousPage = NexVar(1, 6, "previous_page");

  //UI Callback functions
  static void cbNextLocation(void *ptr);
  static void cbPowerSwitchTriggered(void *ptr);
  static void cbAutoSwitchTriggered(void *ptr);
  static void cbSettingsTriggered(void *ptr);

  //Brewster Callback functions
  static void outputChangedEvent(void* callingObject, int outputIdentifier, OutputChangeEvent event);
};
