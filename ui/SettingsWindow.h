#ifndef SETTINGS_WINDOW_H
#define SETTINGS_WINDOW_H

#include "../lib/ui/AWindow.h"
#include "WindowManager.h"

class SettingsWindow : public AWindow {

public:
  SettingsWindow(Adafruit_ILI9341 *tft, AWindowManager *windowManager) : AWindow(tft, windowManager) {};
  ~SettingsWindow() {};
  void initScreen();
  void refreshScreen();
  void process(void* param);
  void processAction(uint8_t action);
  enum Action {SCREEN_CALIBRATION, TEST_OUTPUT, FIND_SENSORS, TEST_SENSORS, W_MAIN_MENU};

protected:


private:
  uint8_t lastMinute = 100;
};

#endif // SETTINGS_WINDOW_H
