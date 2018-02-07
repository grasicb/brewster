#include "MashingWindow.h"
#include "../util/BrewsterGlobals.h"
#include "../controller/BrewsterController.h"

void MashingWindow::initScreen() {
  //tft->fillScreen(ILI9341_BLACK);
  tft->fillScreen(colorBackground);

  //Draw bottom line
//  //tft->setFont(NULL);
//  tft->setFont(font_tiny);
//  tft->setTextSize(1);
//  tft->setCursor(5, 1);
//  tft->print("Brewster Homebrewing System");

  //Draw rectangles with temperature information
  tft->fillRect(10, 10, 80, 80, colorComponentBackground);
  tft->fillRect(100, 10, 80, 80, colorComponentBackground);
  tft->fillRect(190, 10, 80, 80, colorComponentBackground);

  tft->fillRect(10, 100, 80, 80, colorComponentBackground);
  tft->fillRect(100, 100, 80, 80, colorComponentBackground);
  tft->fillRect(190, 100, 80, 80, colorComponentBackground);

  tft->setTextColor(ILI9341_BLACK, colorComponentBackground);
  tft->setTextSize(1);

  tft->setCursor(15, 20);
	tft->print("Drozga");
  tft->setCursor(105, 20);
	tft->print("Vhod");
  tft->setCursor(195, 20);
	tft->print("Izhod");

  //Initialize temperature buffer
  for (int i = 0; i<10; i++) {
    lastTemp[i] = -1000;
  }
  timeInProcess = -1;
  lastMinute = 100;

  //Define and draw buttons
  //buttons.push_back( Button(tft, 215, 180, 90, 35, "Nazaj", Action::W_MAIN_MENU));

  buttons.clear();
  buttons.push_back( Button(tft, 3, 190, 50, 50, icon_play_40x40_bits, icon_play_40x40_width, icon_play_40x40_height, Action::A_START));
  buttons.push_back( Button(tft, 56, 190, 50, 50, icon_stop_bits, icon_stop_width, icon_stop_height, Action::A_STOP));
  buttons.push_back( Button(tft, 109, 190, 50, 50, icon_pump_bits, icon_pump_width, icon_pump_height, Action::A_PUMP1));
  buttons.push_back( Button(tft, 162, 190, 50, 50, icon_pump_bits, icon_pump_width, icon_pump_height, Action::A_PUMP2));
  buttons.push_back( Button(tft, 215, 190, 50, 50, icon_settings_bits, icon_settings_width, icon_settings_height, Action::W_SETTINGS));
  buttons.push_back( Button(tft, 268, 190, 50, 50, icon_menu_bits, icon_menu_width, icon_menu_height, Action::W_MAIN_MENU));

  for(Button b:buttons) {
    b.draw();
  }
  //drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[],int16_t w, int16_t h, uint16_t color),

/*
  tft->fillRoundRect(115, 150, 60, 60, 4, colorButtonNormal);
  tft->drawXBitmap(125, 160, icon_play_40x40_bits, icon_play_40x40_width, icon_play_40x40_height, colorFont);


  tft->drawXBitmap(15, 150, xlogo64_bits, xlogo64_width, xlogo64_height, colorFont);
  */
}

void MashingWindow::refreshScreen() {
  tft->setTextColor(colorFont, colorComponentBackground);
  tft->setTextSize(2);
  tft->setFont(NULL);

  if (BrewsterController::get()->temperatureSensors->temperature[0] != lastTemp[0]) {
    lastTemp[0] = BrewsterController::get()->temperatureSensors->temperature[0];
    tft->setCursor(17, 40);
  	tft->printf("%.02f", BrewsterController::get()->temperatureSensors->temperature[0]);
  }

  if (BrewsterController::get()->temperatureSensors->temperature[1] != lastTemp[1]) {
    lastTemp[1] = BrewsterController::get()->temperatureSensors->temperature[1];
    tft->setCursor(107, 40);
    tft->printf("%.02f", BrewsterController::get()->temperatureSensors->temperature[1]);
  }

  if (BrewsterController::get()->temperatureSensors->temperature[2] != lastTemp[2]) {
    lastTemp[2] = BrewsterController::get()->temperatureSensors->temperature[2];
    tft->setCursor(197, 40);
	  tft->printf("%.02f", BrewsterController::get()->temperatureSensors->temperature[2]);
  }
}

void MashingWindow::process(void* param) {
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


  if (controller->getActiveProcess() == BrewProcesses::MASHING) {

    /////////////////////////////////
    //Timer of the mashing time
    int deltaTime = (Time.now() - controller->getProcessStartTime())/60;

    if (deltaTime != timeInProcess) {
      timeInProcess = deltaTime;

      tft->setTextColor(colorFont, colorComponentBackground);
      tft->setTextSize(2);
      tft->setFont(NULL);
      tft->setCursor(17, 120);
  	  tft->printf("%d m", timeInProcess);
    }

    //////////////////////////////////
    // Logging of timings
    if ((lastLoggedTime + MASH_LOG_INTERVAL) < Time.now()) {
      //Log.trace("%d, %d, %d, %d", lastLoggedTime, Time.now(), controller->getProcessStartTime(), (lastLoggedTime + MASH_LOG_INTERVAL));
      lastLoggedTime = Time.now();
      Log.trace("Mash: %d seconds, %.02f", lastLoggedTime - controller->getProcessStartTime(), BrewsterController::get()->temperatureSensors->temperature[2]);

      Particle.publish("MASHING", String::format("Mash|%d|%.02f|%.02f|%.02f", lastLoggedTime - controller->getProcessStartTime(), BrewsterController::get()->temperatureSensors->temperature[0], BrewsterController::get()->temperatureSensors->temperature[1], BrewsterController::get()->temperatureSensors->temperature[2]));
    }


  // Clean-up time in process if not empty
  }else if (timeInProcess != -1) {
    timeInProcess = -1;
    tft->setTextColor(colorFont, colorComponentBackground);
    tft->setTextSize(2);
    tft->setFont(NULL);
    tft->setCursor(17, 120);
	  tft->print("     ");
  }

  //Refresh data on screen based on defined interval (refreshRate)
  if ((lastRefresh + refreshRate) < millis()) {
    refreshScreen();
    lastRefresh = millis();
  }
}

void MashingWindow::processAction(uint8_t action) {
  Log.trace("Triggering action %03d in window Mashing", action);
  switch (action) {
    case Action::W_MAIN_MENU:
      windowManager->openWindow(WindowManager::Windows::MAIN_WINDOW);
      break;
    case Action::A_START:
      controller->startProcess(BrewProcesses::MASHING);
      break;
    case Action::A_STOP:
      controller->stopProcess();
      break;
    case Action::A_PUMP1:
      statusPump1 = !statusPump1;
      if (statusPump1) {
        buttons[2].setFontColor(ILI9341_GREEN);
      }else {
        buttons[2].setFontColor(ILI9341_RED);
      }
      buttons[2].draw();
      break;
    case Action::A_PUMP2:
      statusPump2 = !statusPump2;
      if (statusPump2) {
        buttons[3].setFontColor(ILI9341_GREEN);
      }else {
        buttons[3].setFontColor(ILI9341_RED);
      }
      buttons[3].draw();
      break;
    default:
      Log.warn("No all actions implemented in window Main Window");
      break;
  }
}
