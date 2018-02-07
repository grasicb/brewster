#pragma once

#include "../lib/ui/AWindow.h"
#include "WindowManager.h"
#include "../controller/BrewsterController.h"

#define MASH_LOG_INTERVAL 9

class MashingWindow : public AWindow {

public:
  MashingWindow(Adafruit_ILI9341 *tft, AWindowManager *windowManager) : AWindow(tft, windowManager) { controller = BrewsterController::get(); };
  ~MashingWindow() {};
  void initScreen();
  void refreshScreen();
  void process(void* param);
  void processAction(uint8_t action);
  enum Action {W_MAIN_MENU, A_START, A_STOP, A_PUMP1, A_PUMP2, W_SETTINGS};

protected:


private:
  uint8_t lastMinute = 100;
  uint16_t timeInProcess = -1;
  int lastLoggedTime = 0;
  uint16_t backgroundColor = 0xD687;
  float lastTemp[10];
  static const uint16_t refreshRate = 100;
  unsigned long lastRefresh = 0;

  boolean statusPump1 = false;
  boolean statusPump2 = false;

  BrewsterController *controller;
};
