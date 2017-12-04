#ifndef WINDOW_MANAGER_H
#define WINDOW_MANAGER_H

#include <map>

#include "../lib/ui/AWindow.h"
#include "../lib/ui/AWindowManager.h"
#include "../lib/lcd/Adafruit_ILI9341.h"

#include "MainWindow.h"
#include "SettingsWindow.h"
#include "MashingWindow.h"
#include "FermentationWindow.h"
#include "TempSensorsTestWindow.h"
#include "EnterValueWindow.h"

#include "icons.h"
#include "../lib/ui/colors.h"

class WindowManager : public AWindowManager {

public:
  enum Windows {MAIN_WINDOW, WINDOW_MASHING, WINDOW_BOLING, WINDOW_FERMENTING, WINDOW_SETTINGS, WINDOW_TEMP_SENSORS_TEST, ENTER_VALUE};
  WindowManager(Adafruit_ILI9341 *tft) : AWindowManager(tft) {};
  void openWindow(uint8_t windowId);
  void destroyWindow(uint8_t windowId);
  void process();
  void screenTouched(uint16_t x, uint16_t y);
  void screenReleased();
  AWindow *getActiveWindow();

protected:


private:
  Logger logger;
  AWindow *currentWindow = NULL;
  std::map<Windows, AWindow *> windows;
};

#endif // WINDOW_MANAGER_H
