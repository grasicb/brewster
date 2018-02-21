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
              NexDSButton(5, 7, "bt1"),
              NexDSButton(5, 8, "bt2"),
              NexDSButton(5, 9, "bt3"),
              NexDSButton(5, 10, "bt4")};

  NexDSButton bAutoSwitches[4] = {
              NexDSButton(5, 11, "bt5"),
              NexDSButton(5, 12, "bt6"),
              NexDSButton(5, 13, "bt7"),
              NexDSButton(5, 14, "bt8")};

  NexDSButton bSettings[4] = {
              NexDSButton(5, 19, "bt9"),
              NexDSButton(5, 20, "bt10"),
              NexDSButton(5, 21, "bt11"),
              NexDSButton(5, 22, "bt12")};

  NexNumber nValues[4] = {
              NexNumber(5, 23, "n1"),
              NexNumber(5, 24, "n2"),
              NexNumber(5, 25, "n3"),
              NexNumber(5, 26, "n4")};

  NexNumber nTemperature = NexNumber(5, 27, "n_temp");
  NexText tLocation = NexText(5, 29, "t_temp_loc");
  NexButton bNextLocation = NexButton(5, 30, "b_next_sensor");
  NexVar vPreviousPage = NexVar(1, 6, "previous_page");

  //UI Callback functions
  static void cbNextLocation(void *ptr);
  static void cbPowerSwitchTriggered(void *ptr);
  static void cbAutoSwitchTriggered(void *ptr);
  static void cbSettingsTriggered(void *ptr);

  //Brewster Callback functions
  static void outputChangedEvent(void* callingObject, int outputIdentifier, OutputChangeEvent event);
};
