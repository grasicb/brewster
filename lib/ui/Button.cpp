#include "Button.h"
//#include "Adafruit_ILI9341.h"
//#include "Adafruit_mfGFX.h"

//#define TFT_GRID

Button::Button() {
  x = 0;
  y = 0;
  width = 0;
  height = 0;
  caption = "";
}

Button::Button(Adafruit_ILI9341* tft, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String caption, uint8_t action) {
  this->tft = tft;
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->caption = caption;
  this->action = action;
}

void Button::updateButton(Adafruit_ILI9341* tft, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String caption, uint8_t action) {
  this->tft = tft;
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->caption = caption;
  this->action = action;
  draw();
}

void Button::draw() {
  tft->fillRect(x, y, width, height, backgroundColor);
  tft->setCursor(x+5, y+(height/2)-4);
	tft->setTextColor(ILI9341_BLACK, backgroundColor);
  tft->setTextSize(1);
	tft->print(caption);
}

boolean Button::isClicked(uint16_t x_pos, uint16_t y_pos) {
  if (x_pos >= x && x_pos <= x+width && y_pos >= y && y_pos <= y + height) {
    if (!isActivated) {
      tft->fillRect(x, y, width, height, ILI9341_RED);
      tft->setCursor(x+5, y+(height/2)-4);
    	tft->setTextColor(ILI9341_BLACK, ILI9341_RED);
      tft->setTextSize(1);
    	tft->print(caption);
      isActivated = true;
    }
    return true;
  } else {
    if (isActivated) {
      tft->fillRect(x, y, width, height, backgroundColor);
      tft->setCursor(x+5, y+(height/2)-4);
    	tft->setTextColor(ILI9341_BLACK, backgroundColor);
      tft->setTextSize(1);
    	tft->print(caption);
      isActivated = false;
    }
    return false;
  }
}

void Button::deactivateButton() {
  if (isActivated) {
    tft->fillRect(x, y, width, height, backgroundColor);
    tft->setCursor(x+5, y+(height/2)-4);
    tft->setTextColor(ILI9341_BLACK, backgroundColor);
    tft->setTextSize(1);
    tft->print(caption);
    isActivated = false;
  }
}

uint8_t Button::getAction() {
  return action;
}
