#ifndef A_WINDOW_MANAGER_H
#define A_WINDOW_MANAGER_H

#include "application.h"
#include "../lcd/Adafruit_ILI9341.h"

#include "../lcd/Fonts/FreeMonoBold9pt7b.h"
#include "../lcd/Fonts/FreeSans9pt7b.h"
#include "../lcd/Fonts/FreeSans12pt7b.h"
#include "../lcd/Fonts/FreeSans18pt7b.h"
//#include "../lcd/Fonts/Picopixel.h"

class AWindowManager {

public:
  AWindowManager(Adafruit_ILI9341 *tft) { this->tft = tft;};
  virtual void openWindow(uint8_t windowId);
  virtual void destroyWindow(uint8_t windowId);
  virtual void process();

  const GFXfont *font_standard = &FreeSans12pt7b;
  const GFXfont *font_small = &FreeSans9pt7b;
  const GFXfont *font_large = &FreeSans18pt7b;
  const GFXfont *font_tiny = NULL;
  const GFXfont *font_monospace = &FreeMonoBold9pt7b;

protected:
  Adafruit_ILI9341 *tft;

private:

};

#endif A_WINDOW_MANAGER_H
