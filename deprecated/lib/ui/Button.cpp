#include "Button.h"
//#include "Adafruit_ILI9341.h"
//#include "Adafruit_mfGFX.h"

//#define TFT_GRID
Button::Button(Adafruit_ILI9341* tft, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String caption, uint8_t action) {
  this->tft = tft;
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->caption = caption;
  this->fontSize = 1;
  this->action = action;

  //Initialize calculated values
  type = ButtonType::BTN_TEXT;
  contentX = x + 6;
  contentY = y + ((height/2)-3);
}

Button::Button(Adafruit_ILI9341* tft, uint16_t x, uint16_t y, uint16_t width, uint16_t height, String caption, uint8_t fontSize, uint8_t action) {
  this->tft = tft;
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->caption = caption;
  this->fontSize = fontSize;
  this->action = action;

  //Initialize calculated values
  type = ButtonType::BTN_TEXT;
  contentX = x + 6;
  contentY = y + ((height/2)-3);
}

Button::Button(Adafruit_ILI9341* tft, uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t imageBitmap[], int16_t imageWidth, int16_t imageHeight, uint8_t action) {
  this->tft = tft;
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
  this->action = action;
  this->imageBitmap = (uint8_t*) imageBitmap;
  this->imageWidth = imageWidth;
  this->imageHeight = imageHeight;

  //Initialize calculated values
  type = ButtonType::BTN_ICON;
  contentX = x + max((width - imageWidth)/2,0);
  contentY = y + max((height - imageHeight)/2,0);
}

void Button::draw() {
  if (fontColor == NULL)
    fontColor = colorButtonFont;

  if (isActivated)
    tft->fillRoundRect(x, y, width, height, 4, colorButtonSelected);
  else
    tft->fillRoundRect(x, y, width, height, 4, colorButtonNormal);


  if (type == ButtonType::BTN_TEXT) {
    tft->setCursor(contentX, contentY);
    if (isActivated)
      tft->setTextColor(fontColor, colorButtonSelected);
    else
      tft->setTextColor(fontColor, colorButtonNormal);
    tft->setFont(font_button);
    tft->setTextSize(fontSize);
    tft->print(caption);


  }else if (type == ButtonType::BTN_ICON) {
    tft->drawXBitmap(contentX, contentY, imageBitmap, imageWidth, imageHeight, fontColor);
  }
}

boolean Button::isClicked(uint16_t x_pos, uint16_t y_pos) {
  lastState = isActivated;

  if (x_pos >= x && x_pos <= x+width && y_pos >= y && y_pos <= y + height) {
    isActivated = true;
  } else {
    isActivated = false;
  }

  // If button state has changed, then redraw the button
  if (lastState != isActivated)
    draw();

  return isActivated;
}

void Button::deactivateButton() {
  lastState = isActivated;
  isActivated = false;
  draw();
}

boolean Button::isPressed() {
  return isActivated;
}

boolean Button::justPressed() {
  if (lastState == false && isActivated == true)
    return true;
  else
    return false;
}

boolean Button::justReleased() {
  if (lastState == true && isActivated == false)
    return true;
  else
    return false;
}

uint8_t Button::getAction() {
  return action;
}

void Button::setFontColor(uint16_t fontColor) {
  this->fontColor = fontColor;
}
