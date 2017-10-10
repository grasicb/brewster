#ifndef BUTTON_H
#define BUTTON_H

#include "../lcd/Adafruit_ILI9341.h"
#include "../lcd/Adafruit_mfGFX.h"

class Button {
public:
  Button();
  Button(Adafruit_ILI9341* tft, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String caption, uint8_t action);
  void updateButton(Adafruit_ILI9341* tft, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String caption, uint8_t action);
  boolean isClicked(uint16_t x_pos, uint16_t y_pos);
  void deactivateButton();
  void draw();
  uint8_t getAction();

private:
  Adafruit_ILI9341* tft;
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  String caption;
  uint8_t action;
  boolean isActivated = false;

  uint16_t backgroundColor = 0xD687;
};

#endif BUTTON_H
