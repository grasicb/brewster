#ifndef TEMPERATURE_SCREEN_H
#define TEMPERATURE_SCREEN_H

#include "../lib/lcd/Adafruit_ILI9341.h"
#include "../lib/lcd/Adafruit_mfGFX.h"
#include "../lib/ui/UI_Button.h"

class TemperatureScreen {
public:
  TemperatureScreen();
  void showLoadingScreen();
  void initScreen();
  Adafruit_ILI9341* getTft();
  void displayData(float humidity, float temp1, float temp2);
  void updateTime(String time);
  void drawPixel(uint16_t x, uint16_t y);
  void screenTouched(uint16_t x, uint16_t y);
  void screenReleased();

private:
  //Adafruit_ILI9341 tft;
  Adafruit_ILI9341 tft = Adafruit_ILI9341(A2, A0, A1);
  //#define NUM_BUTTONS 2
  static const uint16_t NUM_BUTTONS = 2;
  UI_Button *buttons;
  int8_t buttonPressed = -1;
};

#endif TEMPERATURE_SCREEN_H
