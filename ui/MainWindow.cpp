#include "MainWindow.h"
#include "../util/BrewsterGlobals.h"

void MainWindow::initScreen() {
  tft->fillScreen(0x346E);

  tft->setTextSize(1);
  tft->setTextColor(ILI9341_WHITE, 0x346E);
  tft->setCursor(15, 225);
  tft->setFont(windowManager->font_tiny);
  tft->print("Brewster Homebrewing System");

  tft->setFont(windowManager->font_small);
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
      tft->setFont(windowManager->font_tiny);
      tft->setTextColor(ILI9341_WHITE, 0x346E);
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
    case Action::W_MASHING:
      windowManager->openWindow(WindowManager::Windows::WINDOW_MASHING);
      break;
    case Action::W_FERMENTING:
      {
        uint32_t freemem = System.freeMemory();

        Log.trace("Test windows optimisation. Free mem: %d", freemem);
        boolean display_change[320][240];

        freemem = System.freeMemory();
        Log.trace("Create display change array. Free mem: %d", freemem);
        for (int i=0; i<320; i++) {
          for (int j=0; j<240;j++) {
            //if (rand()%2 == 1)
            display_change[i][j] = (boolean)(rand()%2);
          }
        }

        uint16_t display[320][240];

        freemem = System.freeMemory();
        Log.trace("Performance tests. Free mem: %d", freemem);

        long start = millis();
        for (int i=0; i<320; i++) {
          for (int j=0; j<240;j++) {
            display[i][j] = 0xFF;
          }
        }
        long stop = millis();
        Log.trace("Iterating over display array and setting value: %d ms", (stop-start));

        start = millis();
        long counter = 0;
        for (int i=0; i<320; i++) {
          for (int j=0; j<240;j++) {
            if (display_change[i][j]==true) {
//              counter++;
            }
          }
        }
        stop = millis();
        Log.trace("Checking display change (hits %d): %d ms", counter, (stop-start));


        freemem = System.freeMemory();
        Log.trace("Done. Free mem: %d", freemem);
      }
      break;
    case Action::W_BOLING:
//      tft->test();
      break;
    default:
      Log.warn("No all actions implemented in window Main Window");
      break;
  }
}
