#include "EnterValueWindow.h"
#include "../util/BrewsterGlobals.h"
#include "../controller/BrewsterController.h"

void EnterValueWindow::initScreen() {
  tft->fillScreen(colorBackground);

  //Draw rectangles with temperature information
  tft->fillRect(105, 50, 110, 50, colorComponentBackground);

  tft->setTextColor(ILI9341_BLACK, colorComponentBackground);
  tft->setTextSize(3);

  tft->setCursor(115, 65);
  tft->printf("%5.1f", value);


  //Define and draw buttons
  buttons.clear();
  buttons.push_back( Button(tft, 225, 50, 50, 50, "+", 3, 255));
  buttons.push_back( Button(tft, 45, 50, 50, 50, "-", 3, 255));
  buttons.push_back( Button(tft, 105, 150, 50, 50, icon_play_40x40_bits, icon_play_40x40_width, icon_play_40x40_height, Action::A_OK));
  buttons.push_back( Button(tft, 165, 150, 50, 50, icon_stop_bits, icon_stop_width, icon_stop_height, Action::A_CANCEL));

  for(Button b:buttons) {
    b.draw();
  }
}

void EnterValueWindow::refreshScreen() {
  tft->setTextColor(colorFont, colorComponentBackground);
  tft->setTextSize(3);
  tft->setFont(NULL);

  if (buttonPressed == 0 || buttonPressed == 1) {
    if (increaseStarted == 0) {
      increaseStarted = millis();
    }

    //Set increment step (higher increment for longer press)
    float step;
    if (increaseStarted + majorIncrease < millis()) {
      step = majorIncrement;
    } else {
      step = minorIncrement;
    }

    //Increment if resolution correct
    if (lastIncrease == 0 || lastIncrease + minorIncrease < millis()) {
      if (buttonPressed == 0) {
        value += step;
      } else {
        value -= step;
      }

      tft->setCursor(115, 65);
      tft->printf("%5.1f", value);
      lastIncrease = millis();
    }
  }
}

void EnterValueWindow::process(void* param) {
  if ((lastRefresh + refreshRate) < millis()) {
    refreshScreen();
    lastRefresh = millis();
  }
}

void EnterValueWindow::processAction(uint8_t action) {
  Log.trace("Triggering action %03d in window EnterValueWindow", action);
  switch (action) {
    case Action::A_OK:
      windowManager->openWindow(WindowManager::Windows::MAIN_WINDOW);
      break;
    case Action::A_CANCEL:
      if (minorIncrement == 0.5) {
        minorIncrease = 200;
        majorIncrease = 2000;
        minorIncrement = 5;
        majorIncrement = 5;
        value = 50;

      }else {
        minorIncrease = 200;
        majorIncrease = 3100;
        minorIncrement = 0.5;
        majorIncrement = 1;
        value = 50;
      }

      tft->setTextColor(ILI9341_BLACK, colorComponentBackground);
      tft->setTextSize(3);
      tft->setCursor(115, 65);
      tft->printf("%5.1f", value);

      break;

    default:
      Log.warn("Action %03d not implemented in window EnterValueWindow", action);
      increaseStarted = 0;
      break;
  }
}
