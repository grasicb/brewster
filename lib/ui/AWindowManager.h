#ifndef A_WINDOW_MANAGER_H
#define A_WINDOW_MANAGER_H

#include "application.h"
#include "../lcd/Adafruit_ILI9341.h"
#include "fonts.h"


class AWindowManager {

public:
  AWindowManager(Adafruit_ILI9341 *tft) { this->tft = tft;};
  virtual void openWindow(uint8_t windowId);
  virtual void destroyWindow(uint8_t windowId);
  virtual void process();

protected:
  Adafruit_ILI9341 *tft;

private:

};

#endif A_WINDOW_MANAGER_H
