#pragma once

#include "../lib/ui/AWindow.h"
#include "WindowManager.h"
#include "../controller/BrewsterController.h"

#define LOG_INTERVAL 599 //Log temperature every 10 min

class FermentationWindow : public AWindow {

public:
  FermentationWindow(Adafruit_ILI9341 *tft, AWindowManager *windowManager) : AWindow(tft, windowManager) { controller = BrewsterController::get(); };
  ~FermentationWindow() {};
  void initScreen();
  void refreshScreen();
  void process(void* param);
  void processAction(uint8_t action);
  enum Action {W_MAIN_MENU, A_START, A_STOP, W_SETTINGS};

protected:


private:
  uint8_t lastMinute = 100;
  uint16_t timeInProcess = -1;
  int lastLoggedTime = 0;
//  uint16_t backgroundColor = 0xD687;
  float lastTemp[10];

  BrewsterController *controller;
};
