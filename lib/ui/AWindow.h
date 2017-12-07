#ifndef AWINDOW_H
#define AWINDOW_H


#include "../lcd/Adafruit_ILI9341.h"
#undef min
#undef max
#include "Button.h"
#include "AWindowManager.h"
#include <vector>
#include "application.h"


class AWindow {
public:
  AWindow(Adafruit_ILI9341 *tft, AWindowManager *windowManager) { this->tft=tft; this->windowManager=windowManager;};
  ~AWindow() {};
  virtual void initScreen();
  virtual void refreshScreen();
  virtual void process(void* param);
  virtual void processAction(uint8_t action);

  void screenTouched(uint16_t x, uint16_t y);
  void screenReleased();

protected:
  Adafruit_ILI9341 *tft;
  std::vector<Button> buttons;
  int8_t buttonPressed = -1;
  int8_t lastButtonPressed = -1;
  AWindowManager *windowManager;

private:
  Logger logger;
};

#endif // AWINDOW_H
