#include "SettingsWindow.h"
#include "../util/BrewsterGlobals.h"
#include "../util/TempUtils.h"

void SettingsWindow::initScreen() {
  lcdMutex.lock();
  tft->fillScreen(ILI9341_BLACK);

  tft->setTextSize(1);
  tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft->setCursor(15, 225);
  tft->print("Brewster Homebrewing System");
  lcdMutex.unlock();

  buttons.push_back( Button(tft, 25, 25, 125, 35, "Kalibracija ekrana", Action::SCREEN_CALIBRATION));
  buttons.push_back( Button(tft, 170, 25, 125, 35, "Test izhodov", Action::TEST_OUTPUT));
  buttons.push_back( Button(tft, 25, 80, 125, 35, "Iskanje senzorjev", Action::FIND_SENSORS));
  buttons.push_back( Button(tft, 170, 80, 125, 35, "Test senzorjev", Action::TEST_SENSORS));
  buttons.push_back( Button(tft, 170, 135, 125, 35, "Nazaj", Action::W_MAIN_MENU));

  for(Button b:buttons) {
    b.draw();
  }
  //startProcessing();
  //new Thread(NULL, thread_process);
}

void SettingsWindow::refreshScreen() {}

void SettingsWindow::process(void* param) {
  if (lastMinute != Time.minute()) {
      lastMinute = Time.minute();
      tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft->setCursor(285, 225);
      tft->printf("%02d:%02d     ", Time.hour(), Time.minute());
  }
}

void SettingsWindow::processAction(uint8_t action) {
  switch (action) {
    case Action::W_MAIN_MENU:
      windowManager->openWindow(WindowManager::Windows::MAIN_WINDOW);
      break;
    case Action::FIND_SENSORS:
      TempUtils::setPrecision(DS18::PRECISION::BIT_9);
      delay(1000);
      TempUtils::listSensors();
      break;
    case Action::TEST_SENSORS:
      TempUtils::readTemperature();
      break;
    default:
      Log.warn("No all actions implemented in window Settings");
      break;
  }
}
