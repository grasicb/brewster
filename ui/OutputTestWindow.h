#pragma once

#include "../lib/ui/AWindow.h"
#include "WindowManager.h"
#include "../controller/BrewsterController.h"

class OutputTestWindow : public AWindow {

public:
  OutputTestWindow(Adafruit_ILI9341 *tft, AWindowManager *windowManager) : AWindow(tft, windowManager) { controller = BrewsterController::get(); };
  ~OutputTestWindow() {};
  void initScreen();
  void refreshScreen();
  void process(void* param);
  void processAction(uint8_t action);
  void processChangeValue();
  enum Action {W_MAIN_MENU, TOGGLE_0, TOGGLE_1, TOGGLE_2, TOGGLE_3
                          , MODE_0, MODE_1, MODE_2, MODE_3
                          , SETTINGS_0, SETTINGS_1, SETTINGS_2, SETTINGS_3};

protected:


private:
  uint8_t lastMinute = 100;
  uint16_t refreshRate = 100;
  unsigned long lastRefresh = 0;

  bool outputStatus[OUTPUT_NUMBER] = {false, false, false, false};
  int outputValue[OUTPUT_NUMBER] = {0, 0, 0, 0};
  bool outputPID[OUTPUT_NUMBER] = {false, false, false, false};

  int outputSetValue[OUTPUT_NUMBER] = {100, 100, 100, 100};
  double outputSetPoint[OUTPUT_NUMBER] = {28, 28, 28, 28};

  int changeValueOutputID = -1;

  uint8_t pidInputId = 3;
  float lastTemp = -100;

  BrewsterController *controller;
};
