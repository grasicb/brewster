#include "TemperatureScreen.h"
//#include "Adafruit_ILI9341.h"
//#include "Adafruit_mfGFX.h"

//#define TFT_GRID

TemperatureScreen::TemperatureScreen() {
  tft.begin();
}

void TemperatureScreen::showLoadingScreen() {
  tft.begin();
  tft.setRotation(3);
  tft.setFont(ARIAL_8);
	tft.fillScreen(ILI9341_WHITE);
	tft.setCursor(50, 110);
	tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);  tft.setTextSize(2);
	tft.println("Zaganjam aplikacijo");
}

Adafruit_ILI9341* TemperatureScreen::getTft() {
  return &tft;
}

void TemperatureScreen::initScreen() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);

#ifdef TFT_GRID
  tft.setTextSize(1);
  uint16_t color;

  for (int i=0; i<320; i=i+10) {
    if (i%50 == 0) {
        color = ILI9341_RED;
    }else {
        color = ILI9341_WHITE;
    }

    tft.drawPixel(i, 0, color);
    if (i != 0) {
      tft.drawPixel(i, 239, color);
    }if (i>0 && i < 240) {
      tft.drawPixel(0, i, color);
      tft.drawPixel(319, i, color);
    }
  }
#endif

  tft.setTextSize(2);
  tft.setCursor(2, 10);
  tft.println("Temperaturni senzor");


  tft.setTextSize(1);
  tft.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
  tft.setCursor(2, 40);
  tft.print("Cas: ");
  tft.setCursor(2, 55);
  tft.print("Vlaga: ");
  tft.setCursor(2, 70);
  tft.print("Temperatura 1: ");
  tft.setCursor(2, 85);
  tft.print("Temperatura 2: ");

//  tft.drawFastHLine(0, 30, 43, ILI9341_YELLOW);

  buttons = new UI_Button[NUM_BUTTONS];
  buttons[0].updateButton(&tft, 15, 200, 80, 30, "Start mashing", "MASHING_START");
  buttons[1].updateButton(&tft, 110, 200, 80, 30, "Stop mashing", "MASHING_STOP");
}

void TemperatureScreen::displayData(float humidity, float temp1, float temp2) {
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_BLUE, ILI9341_BLACK);
  tft.setCursor(34, 55);
  tft.print(humidity);
  tft.print(" %   ");
  tft.setCursor(77, 70);
  tft.print(temp1);
  tft.print(" C   ");
  tft.setCursor(80, 85);
  tft.print(temp2);
  tft.print(" C   ");
}

void TemperatureScreen::updateTime(String time) {
  tft.setCursor(26, 40);
  tft.print(time);
  tft.print("   ");
}

void TemperatureScreen::drawPixel(uint16_t x, uint16_t y) {
  tft.drawPixel(x, y, ILI9341_RED);
}

void TemperatureScreen::screenTouched(uint16_t x, uint16_t y) {
  //Serial.printf("Checking whether buttons have been pressed (x=%d, y=%d)\n", x, y);
  buttonPressed = -1;
  for (int i=0; i<NUM_BUTTONS; i++) {
    if (buttons[i].isClicked(x, y)) {
      //Serial.print("Button clicked. Action: ");
      //Serial.println(buttons[i].getAction());
      buttonPressed = i;
    }
  }
}

  void TemperatureScreen::screenReleased() {
    tft.setCursor(50, 110);
    tft.setTextColor(ILI9341_BLACK, ILI9341_RED);
    if (buttonPressed == -1) {
      tft.print("                                                    ");
    }else{
      tft.print(buttons[buttonPressed].getAction());
      tft.print("             ");
    }

    for (int i=0; i<NUM_BUTTONS; i++) {
      buttons[i].deactivateButton();
    }
    Serial.print("Selected button: ");
    Serial.println(buttonPressed);
  }
