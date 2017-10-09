#include "MainWindow.h"
#include "../util/BrewsterGlobals.h"

void MainWindow::initScreen() {
  lcdMutex.lock();
  tft->fillScreen(ILI9341_BLACK);

  tft->setTextSize(1);
  tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
  tft->setCursor(15, 225);
  tft->print("Brewster Homebrewing System");
  lcdMutex.unlock();

  buttons.push_back( Button(tft, 25, 25, 125, 35, "Drozganje", Action::W_MASHING));
  buttons.push_back( Button(tft, 170, 25, 125, 35, "Kuhanje", Action::W_BOLING));
  buttons.push_back( Button(tft, 25, 80, 125, 35, "Fermentiranje", Action::W_FERMENTING));
  buttons.push_back( Button(tft, 170, 80, 125, 35, "Nastavitve", Action::W_SETTINGS));

  for(Button b:buttons) {
    b.draw();
  }
  //startProcessing();
  //new Thread(NULL, thread_process);
}

void MainWindow::refreshScreen() {}

void MainWindow::process(void* param) {
  if (lastMinute != Time.minute()) {
      lastMinute = Time.minute();
      tft->setTextColor(ILI9341_WHITE, ILI9341_BLACK);
      tft->setCursor(285, 225);
      tft->printf("%02d:%02d     ", Time.hour(), Time.minute());
  }
}

void MainWindow::processAction(uint8_t action) {
  Log.trace("Triggering action %03d in window Main Menu", action);
  switch (action) {
    case Action::W_SETTINGS:
      windowManager->openWindow(WindowManager::Windows::WINDOW_SETTINGS);
      break;
    default:
      Log.warn("No all actions implemented in window Main Window");
      break;
  }
}
