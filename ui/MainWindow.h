#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "../lib/ui/AWindow.h"
#include "WindowManager.h"

class MainWindow : public AWindow {

public:
  MainWindow(Adafruit_ILI9341 *tft, AWindowManager *windowManager) : AWindow(tft, windowManager) {};
  ~MainWindow() {};
  void initScreen();
  void refreshScreen();
  void process(void* param);
  void processAction(uint8_t action);
  enum Action {W_MASHING, W_BOLING, W_FERMENTING, W_SETTINGS};

protected:


private:
  uint8_t lastMinute = 100;
};

#endif MAIN_WINDOW_H
