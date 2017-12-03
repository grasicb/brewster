#include "MainWindow.h"
#include "../util/BrewsterGlobals.h"

void MainWindow::initScreen() {
  tft->fillScreen(0x346E);

  tft->setTextSize(1);
  tft->setTextColor(ILI9341_WHITE, 0x346E);
  tft->setCursor(15, 225);
  tft->setFont(font_tiny);
  tft->print("Brewster Homebrewing System");

  tft->setFont(font_small);
  buttons.clear();
  buttons.push_back( Button(tft, 25, 25, 125, 35, "Drozganje", Action::W_MASHING));
  buttons.push_back( Button(tft, 170, 25, 125, 35, "Kuhanje", Action::W_BOLING));
  buttons.push_back( Button(tft, 25, 80, 125, 35, "Fermentiranje", Action::W_FERMENTING));
  buttons.push_back( Button(tft, 170, 80, 125, 35, "Nastavitve", Action::W_SETTINGS));

  for(Button b:buttons) {
    b.draw();
  }
}

void MainWindow::refreshScreen() {}

void MainWindow::process(void* param) {
  if (lastMinute != Time.minute()) {
      lastMinute = Time.minute();
      tft->setFont(font_tiny);
      tft->setTextColor(ILI9341_WHITE, 0x346E);
      tft->setCursor(285, 225);
      tft->printf("%02d:%02d     ", Time.hour(), Time.minute());
  }

  if (activeProcess != controller->getActiveProcess()) {
    activeProcess = controller->getActiveProcess();
    tft->setFont(font_small);
    tft->setTextColor(ILI9341_BLACK, 0x346E);
    tft->setCursor(15, 180);
    tft->printf("Active process: %d   ", activeProcess);
  }
}

void MainWindow::processAction(uint8_t action) {
  Log.trace("Triggering action %03d in window Main Menu", action);
  switch (action) {
    case Action::W_SETTINGS:
      windowManager->openWindow(WindowManager::Windows::WINDOW_SETTINGS);
      break;
    case Action::W_MASHING:
      windowManager->openWindow(WindowManager::Windows::WINDOW_MASHING);
      break;
    case Action::W_FERMENTING:
      windowManager->openWindow(WindowManager::Windows::WINDOW_FERMENTING);
      break;
    case Action::W_BOLING:
//      tft->test();
      break;
    default:
      Log.warn("No all actions implemented in window Main Window");
      break;
  }
}
