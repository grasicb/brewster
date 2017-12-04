#pragma once

#include "../lib/ui/AWindow.h"
#include "WindowManager.h"
#include "../controller/BrewsterController.h"

class EnterValueWindow : public AWindow {

public:
  EnterValueWindow(Adafruit_ILI9341 *tft, AWindowManager *windowManager) : AWindow(tft, windowManager) { controller = BrewsterController::get(); };
  ~EnterValueWindow() {};
  void initScreen();
  void refreshScreen();
  void process(void* param);
  void processAction(uint8_t action);
  enum Action {A_OK, A_CANCEL};

protected:


private:

  uint16_t refreshRate = 50;
  uint16_t minorIncrease = 200;
  uint16_t majorIncrease = 3000;
  float minorIncrement = 0.5;
  float majorIncrement = 1;
  unsigned long lastRefresh = 0;
  unsigned long increaseStarted = 0;
  unsigned long lastIncrease = 0;
  float value = 50;

  BrewsterController *controller;
};
