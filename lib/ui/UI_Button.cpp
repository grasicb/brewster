#include "UI_Button.h"
//#include "Adafruit_ILI9341.h"
//#include "Adafruit_mfGFX.h"

//#define TFT_GRID

UI_Button::UI_Button() {
  x = 0;
  y = 0;
  width = 0;
  height = 0;
  caption = "";
}

UI_Button::UI_Button(Adafruit_ILI9341* tft, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String caption, String action) {
  this->tft = tft;
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->caption = caption;
  this->action = action;
}

void UI_Button::updateButton(Adafruit_ILI9341* tft, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String caption, String action) {
  this->tft = tft;
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->caption = caption;
  this->action = action;
  draw();
}

void UI_Button::draw() {
  tft->fillRect(x, y, width, height, ILI9341_YELLOW);
  tft->setCursor(x+5, y+(height/2)-4);
	tft->setTextColor(ILI9341_BLACK, ILI9341_YELLOW);
  tft->setTextSize(1);
	tft->print(caption);
}

boolean UI_Button::isClicked(uint16_t x_pos, uint16_t y_pos) {
  //Serial.print("Button ");
  //Serial.print(action);
  //Serial.printf(" (x=%d, y=%d, w=%d, h=%d)\n", x, y, width, height);

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
      tft->fillRect(x, y, width, height, ILI9341_YELLOW);
      tft->setCursor(x+5, y+(height/2)-4);
    	tft->setTextColor(ILI9341_BLACK, ILI9341_YELLOW);
      tft->setTextSize(1);
    	tft->print(caption);
      isActivated = false;
    }
    return false;
  }
}

void UI_Button::deactivateButton() {
  if (isActivated) {
    tft->fillRect(x, y, width, height, ILI9341_YELLOW);
    tft->setCursor(x+5, y+(height/2)-4);
    tft->setTextColor(ILI9341_BLACK, ILI9341_YELLOW);
    tft->setTextSize(1);
    tft->print(caption);
    isActivated = false;
  }
}

String UI_Button::getAction() {
  return action;
}
