#pragma once

#include "AWindowController.h"
#include "../controller/TemperatureSensor.h"
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
  NexNumber nO1Value = NexNumber(5, 23, "n1");
  NexNumber nO2Value = NexNumber(5, 24, "n2");
  NexNumber nO3Value = NexNumber(5, 25, "n3");
  NexNumber nO4Value = NexNumber(5, 26, "n4");

  NexNumber nTemperature = NexNumber(5, 27, "n_temp");
  NexText tLocation = NexText(5, 29, "t_temp_loc");

  NexDSButton bO1PowerSwitch = NexDSButton(5, 7, "bt1");
  NexDSButton bO2PowerSwitch = NexDSButton(5, 8, "bt2");
  NexDSButton bO3PowerSwitch = NexDSButton(5, 9, "bt3");
  NexDSButton bO4PowerSwitch = NexDSButton(5, 10, "bt4");

  NexDSButton bO1AutoSwitch = NexDSButton(5, 11, "bt5");
  NexDSButton bO2AutoSwitch = NexDSButton(5, 12, "bt6");
  NexDSButton bO3AutoSwitch = NexDSButton(5, 13, "bt7");
  NexDSButton bO4AutoSwitch = NexDSButton(5, 14, "bt8");

  NexDSButton bO1Settings = NexDSButton(5, 19, "bt9");
  NexDSButton bO2Settings = NexDSButton(5, 20, "bt10");
  NexDSButton bO3Settings = NexDSButton(5, 21, "bt11");
  NexDSButton bO4Settings = NexDSButton(5, 22, "bt12");

  NexButton bNextLocation = NexButton(5, 30, "b_next_sensor");

  NexVar vPreviousPage = NexVar(1, 6, "previous_page");

  //UI Callback functions
  static void cbNextLocation(void *ptr);
  static void cbPowerSwitchTriggered(void *ptr);
  static void cbAutoSwitchTriggered(void *ptr);
  static void cbSettingsTriggered(void *ptr);
};
