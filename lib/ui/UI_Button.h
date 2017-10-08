#ifndef UI_BUTTON_H
#define UI_BUTTON_H

#include "../lcd/Adafruit_ILI9341.h"
#include "../lcd/Adafruit_mfGFX.h"

class UI_Button {
public:
  UI_Button();
  UI_Button(Adafruit_ILI9341* tft, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String caption, String action);
  void updateButton(Adafruit_ILI9341* tft, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String caption, String action);
  boolean isClicked(uint16_t x_pos, uint16_t y_pos);
  void deactivateButton();
  void draw();
  String getAction();

private:
  Adafruit_ILI9341* tft;
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  String caption;
  String action;
  boolean isActivated = false;
};

#endif UI_BUTTON_H
