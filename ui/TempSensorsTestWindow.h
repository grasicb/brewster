#pragma once

#include "../lib/ui/AWindow.h"
#include "WindowManager.h"
#include "../controller/BrewsterController.h"

class TempSensorsTestWindow : public AWindow {

public:
  TempSensorsTestWindow(Adafruit_ILI9341 *tft, AWindowManager *windowManager) : AWindow(tft, windowManager) { controller = BrewsterController::get(); };
  ~TempSensorsTestWindow() {};
  void initScreen();
  void refreshScreen();
  void process(void* param);
  void processAction(uint8_t action);
  enum Action {W_MAIN_MENU};

protected:


private:
  uint8_t lastMinute = 100;
  float lastTemp[10];
  const uint16_t refreshRate = 100;
  unsigned long lastRefresh = 0;

  BrewsterController *controller;
};
