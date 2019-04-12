#include "FermentationWindow.h"
#include "../util/BrewsterGlobals.h"
#include "../controller/BrewsterController.h"

void FermentationWindow::initScreen() {
  tft->fillScreen(colorBackground);

  //Draw rectangles with temperature information
  tft->fillRect(35, 32, 125, 125, colorComponentBackground);
  tft->fillRect(170, 32, 125, 125, colorComponentBackground);

  tft->setTextColor(ILI9341_BLACK, colorComponentBackground);
  tft->setTextSize(2);

  tft->setCursor(40, 45);
	tft->print("Pivo");
  tft->setCursor(175, 45);
	tft->print("Prostor");

  //Initialize temperature buffer
  for (int i = 0; i<10; i++) {
    lastTemp[i] = -1000;
  }
  timeInProcess = -1;
  lastMinute = 100;


  //Define and draw buttons
  buttons.clear();
  buttons.push_back( Button(tft, 35, 190, 50, 50, icon_play_40x40_bits, icon_play_40x40_width, icon_play_40x40_height, Action::A_START));
  buttons.push_back( Button(tft, 88, 190, 50, 50, icon_stop_bits, icon_stop_width, icon_stop_height, Action::A_STOP));
  buttons.push_back( Button(tft, 192, 190, 50, 50, icon_settings_bits, icon_settings_width, icon_settings_height, Action::W_SETTINGS));
  buttons.push_back( Button(tft, 245, 190, 50, 50, icon_menu_bits, icon_menu_width, icon_menu_height, Action::W_MAIN_MENU));

  for(Button b:buttons) {
    b.draw();
  }
}

void FermentationWindow::refreshScreen() {
  tft->setTextColor(colorFont, colorComponentBackground);
  tft->setTextSize(3);
  tft->setFont(NULL);

  if (BrewsterController::get()->temperatureSensors->temperature[0] != lastTemp[0]) {
    lastTemp[0] = BrewsterController::get()->temperatureSensors->temperature[0];
    tft->setCursor(42, 70);
  	tft->printf("%4.1f", BrewsterController::get()->temperatureSensors->temperature[0]);
  }

  if (BrewsterController::get()->temperatureSensors->temperature[1] != lastTemp[1]) {
    lastTemp[1] = BrewsterController::get()->temperatureSensors->temperature[1];
    tft->setCursor(177, 70);
    tft->printf("%4.1f", BrewsterController::get()->temperatureSensors->temperature[1]);
  }
}

void FermentationWindow::process(void* param) {
  //Update clock
  if (lastMinute != Time.minute()) {
      lastMinute = Time.minute();
      tft->setTextColor(ILI9341_WHITE, colorBackground);
      //tft->setFont(NULL);
      tft->setFont(font_tiny);
      tft->setTextSize(1);
      tft->setCursor(265, 1);
      tft->printf("%02d:%02d", Time.hour(), Time.minute());
  }


  if (controller->getActiveProcess() == BrewProcesses::FERMENTING) {

    /////////////////////////////////
    //Timer of the fermentation time
    int deltaTime = (Time.now() - controller->getProcessStartTime())/60/60/24;

    if (deltaTime != timeInProcess) {
      timeInProcess = deltaTime;

      tft->setTextColor(colorFont, colorComponentBackground);
      tft->setTextSize(2);
      tft->setFont(NULL);
      tft->setCursor(42, 130);
  	  tft->printf("%d dni", timeInProcess);
    }

    //////////////////////////////////
    // Logging of timings
    /*
    if ((lastLoggedTime + LOG_INTERVAL) < Time.now()) {
      //Log.trace("%d, %d, %d, %d", lastLoggedTime, Time.now(), controller->getProcessStartTime(), (lastLoggedTime + MASH_LOG_INTERVAL));
      lastLoggedTime = Time.now();
      Log.trace("Fermenting: %d minutes, %5.2f", (lastLoggedTime - controller->getProcessStartTime())/60, BrewsterController::get()->temperatureSensors->temperature[1]);

      Particle.publish("FEMENTING", String::format("Ferment|%d|%.2f|%.2f|%d:%d", (lastLoggedTime - controller->getProcessStartTime())/60, BrewsterController::get()->temperatureSensors->temperature[0], BrewsterController::get()->temperatureSensors->temperature[1], Time.hour(), Time.minute()));
    }
    */


  // Clean-up time in process if not empty
  }else if (timeInProcess != -1) {
    timeInProcess = -1;
    tft->setTextColor(colorFont, colorComponentBackground);
    tft->setTextSize(2);
    tft->setFont(NULL);
    tft->setCursor(42, 130);
	  tft->print("         ");
  }

  refreshScreen();
}

void FermentationWindow::processAction(uint8_t action) {
  Log.trace("Triggering action %03d in window Fermenting", action);
  switch (action) {
    case Action::W_MAIN_MENU:
      windowManager->openWindow(WindowManager::Windows::MAIN_WINDOW);
      break;
    case Action::A_START:
      controller->startProcess(BrewProcesses::FERMENTING);
      break;
    case Action::A_STOP:
      controller->stopProcess();
      break;
    default:
      Log.warn("No all actions implemented in window Fermenting Window");
      break;
  }
}
