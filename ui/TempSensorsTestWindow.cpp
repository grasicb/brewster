#include "TempSensorsTestWindow.h"
#include "../util/BrewsterGlobals.h"
#include "../controller/BrewsterController.h"

void TempSensorsTestWindow::initScreen() {
  tft->fillScreen(colorBackground);

  //Draw rectangles with temperature information
  tft->fillRect(0, 10, 320, 175, colorComponentBackground);

  tft->setTextColor(ILI9341_BLACK, colorComponentBackground);
  tft->setTextSize(2);

  for (int i = 0; i<10; i++) {
    tft->setCursor(10, 12+(i*16));
  	tft->print(String::format("%2i: ", i+1));
    lastTemp[i] = -1000;
  }
  lastMinute = 100;

  //Define and draw buttons
  buttons.clear();
  buttons.push_back( Button(tft, 270, 190, 50, 50, icon_menu_bits, icon_menu_width, icon_menu_height, Action::W_MAIN_MENU));

  for(Button b:buttons) {
    b.draw();
  }
}

void TempSensorsTestWindow::refreshScreen() {
  tft->setTextColor(colorFont, colorComponentBackground);
  tft->setTextSize(2);
  tft->setFont(NULL);

  for (int i = 0; i<10; i++) {
    if (BrewsterController::get()->temperatureSensors->temperature[i] != lastTemp[i]) {
      lastTemp[i] = BrewsterController::get()->temperatureSensors->temperature[i];
      tft->setCursor(55, 12+(i*16));
      tft->printf("%3.2f", lastTemp[i]);
      Log.trace("Temp %d: %3.2f", i, lastTemp[i]);
    }
  }
}

void TempSensorsTestWindow::process(void* param) {
  //Update clock
  if (lastMinute != Time.minute()) {
      lastMinute = Time.minute();
      tft->setTextColor(ILI9341_WHITE, colorBackground);
      //tft->setFont(NULL);
      tft->setFont(font_tiny);
      tft->setTextSize(1);
      tft->setCursor(285, 1);
      tft->printf("%02d:%02d", Time.hour(), Time.minute());
  }

  if ((lastRefresh + refreshRate) < millis()) {
    refreshScreen();
    lastRefresh = millis();
  }
}

void TempSensorsTestWindow::processAction(uint8_t action) {
  Log.trace("Triggering action %03d in window TempSensorsTestWindow", action);
  switch (action) {
    case Action::W_MAIN_MENU:
      windowManager->openWindow(WindowManager::Windows::MAIN_WINDOW);
      break;
    default:
      Log.error("Action %03d not implemented in window TempSensorsTestWindow", action);
      break;
  }
}
