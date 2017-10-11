#include "MashingWindow.h"
#include "../util/BrewsterGlobals.h"
#include "../controller/BrewsterController.h"

void MashingWindow::initScreen() {
  lcdMutex.lock();
  //tft->fillScreen(ILI9341_BLACK);
  tft->fillScreen(0x346E);

  //Draw bottom line
  tft->setTextSize(1);
  tft->setTextColor(ILI9341_WHITE, 0x346E);
  tft->setCursor(15, 225);
  tft->print("Brewster Homebrewing System");
  lcdMutex.unlock();

  //Draw rectangles with temperature information
  tft->fillRect(15, 45, 90, 90, backgroundColor);
  tft->fillRect(115, 45, 90, 90, backgroundColor);
  tft->fillRect(215, 45, 90, 90, backgroundColor);

  tft->setTextColor(ILI9341_BLACK, backgroundColor);
  tft->setTextSize(1);

  tft->setCursor(25, 55);
	tft->print("Drozga");
  tft->setCursor(125, 55);
	tft->print("Vhod");
  tft->setCursor(225, 55);
	tft->print("Izhod");

  //Define and draw buttons
  buttons.push_back( Button(tft, 215, 180, 90, 35, "Nazaj", Action::W_MAIN_MENU));

  for(Button b:buttons) {
    b.draw();
  }
}

void MashingWindow::refreshScreen() {
  tft->setTextColor(ILI9341_BLACK, backgroundColor);
  tft->setTextSize(2);

  if (BrewsterController::get()->temperatureSensors->temperature[0] != lastTemp[0]) {
    lastTemp[0] = BrewsterController::get()->temperatureSensors->temperature[0];
    tft->setCursor(35, 82);
  	tft->printf("%.02f   ", BrewsterController::get()->temperatureSensors->temperature[0]);
  }

  if (BrewsterController::get()->temperatureSensors->temperature[1] != lastTemp[1]) {
    lastTemp[1] = BrewsterController::get()->temperatureSensors->temperature[1];
    tft->setCursor(135, 82);
    tft->printf("%.02f   ", BrewsterController::get()->temperatureSensors->temperature[1]);
  }

  if (BrewsterController::get()->temperatureSensors->temperature[2] != lastTemp[2]) {
    lastTemp[2] = BrewsterController::get()->temperatureSensors->temperature[2];
    tft->setCursor(235, 82);
	  tft->printf("%.02f   ", BrewsterController::get()->temperatureSensors->temperature[2]);
  }
}

void MashingWindow::process(void* param) {
  if (lastMinute != Time.minute()) {
      lastMinute = Time.minute();
      tft->setTextColor(ILI9341_WHITE, 0x346E);
      tft->setTextSize(1);
      tft->setCursor(285, 225);
      tft->printf("%02d:%02d     ", Time.hour(), Time.minute());
  }

  refreshScreen();
}

void MashingWindow::processAction(uint8_t action) {
  Log.trace("Triggering action %03d in window Mashing", action);
  switch (action) {
    case Action::W_MAIN_MENU:
      windowManager->openWindow(WindowManager::Windows::MAIN_WINDOW);
      break;
    default:
      Log.warn("No all actions implemented in window Main Window");
      break;
  }
}
