#ifndef BUTTON_H
#define BUTTON_H

#include "../lcd/Adafruit_ILI9341.h"
#include "../lcd/Adafruit_GFX.h"
#include "colors.h"
#include "fonts.h"

class Button {
public:
  Button(Adafruit_ILI9341* tft, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String caption, uint8_t action);
  Button(Adafruit_ILI9341* tft, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t imageBitmap[], int16_t imageWidth, int16_t imageHeight, uint8_t action);
  boolean isClicked(uint16_t x_pos, uint16_t y_pos);
  boolean isPressed();
  boolean justPressed();
  boolean justReleased();
  void deactivateButton();
  void draw();
  uint8_t getAction();
  enum ButtonType {BTN_TEXT, BTN_ICON};
  void setFontColor(uint16_t fontColor);

private:
  Adafruit_ILI9341* tft;
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  uint8_t action;

  //Button can have either caption or image
  String caption;
  uint8_t* imageBitmap;
  int16_t imageWidth;
  int16_t imageHeight;

  //Content position
  int16_t contentX;
  int16_t contentY;

  ButtonType type;

  boolean isActivated = false;
  boolean lastState = false;
//  GFXfont *font = NULL;
  uint16_t fontColor = NULL;
  //uint16_t fontColorActive = NULL;
};

#endif BUTTON_H
