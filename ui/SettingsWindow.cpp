#include "SettingsWindow.h"
#include "../util/BrewsterGlobals.h"
#include "../util/BrewsterUtils.h"
#include "../util/TempUtils.h"
#include "../controller/BrewsterController.h"

void SettingsWindow::initScreen() {
  tft->fillScreen(ILI9341_BLACK);

  tft->setTextSize(1);
  tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft->setCursor(15, 225);
  tft->print("Brewster Homebrewing System");

  buttons.clear();
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
      BrewsterUtils::i2c_scanner();

      TempUtils::setPrecision(DS18::PRECISION::BIT_11);
      delay(1000);
      TempUtils::listSensors();

      break;
    case Action::TEST_SENSORS:
      //TempUtils::readTemperature();
      windowManager->openWindow(WindowManager::Windows::WINDOW_TEMP_SENSORS_TEST);
      break;
    case Action::TEST_OUTPUT:
      //TempUtils::readTemperature();
      //windowManager->openWindow(WindowManager::Windows::ENTER_VALUE);
      windowManager->openWindow(WindowManager::Windows::OUTPUT_TEST);
      /*
      if (AC1_Power ==100) {
        AC1_Power = 0;
      }else{
        AC1_Power += 10;
      }
      BrewsterController::get()->getOutput(Outputs::AC1)->setOutput(AC1_Power);
      */

      break;
    default:
      Log.warn("No all actions implemented in window Settings");
      break;
  }
}
