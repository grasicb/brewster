#pragma once

#include "../lib/ui/AWindow.h"
#include "WindowManager.h"

class MashingWindow : public AWindow {

public:
  MashingWindow(Adafruit_ILI9341 *tft, AWindowManager *windowManager) : AWindow(tft, windowManager) {};
  ~MashingWindow() {};
  void initScreen();
  void refreshScreen();
  void process(void* param);
  void processAction(uint8_t action);
  enum Action {W_MAIN_MENU};

protected:


private:
  uint8_t lastMinute = 100;
  uint16_t backgroundColor = 0xD687;
  float lastTemp[10];
};
