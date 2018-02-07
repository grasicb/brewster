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

  drawButtons();
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

      if (refIntValue != NULL) {
        *refIntValue = (int)value;
      }else if (refFloatValue != NULL){
        *refFloatValue = (float)value;
      }else if (refDoubleValue != NULL) {
        *refDoubleValue = value;
      }
      Log.trace("Setting value to %5.2f", value);

      windowManager->openPreviousWindow();
      break;
    case Action::A_CANCEL:
      windowManager->openPreviousWindow();
      break;

    default:
      Log.warn("Action %03d not implemented in window EnterValueWindow", action);
      increaseStarted = 0;
      break;
  }
}


void EnterValueWindow::setLimits(double lowerLimit, double upperLimit) {
  this->lowerLimit = lowerLimit;
  this->upperLimit = upperLimit;
}

void EnterValueWindow::setOutputValue(int *value) {
  refIntValue = value;
  refFloatValue = NULL;
  refDoubleValue = NULL;
  this->value = (double)*value;
}

void EnterValueWindow::setOutputValue(float *value) {
  refIntValue = NULL;
  refFloatValue = value;
  refDoubleValue = NULL;
  this->value = (double)*value;
}

void EnterValueWindow::setOutputValue(double *value) {
  refIntValue = NULL;
  refFloatValue = NULL;
  refDoubleValue = value;
  this->value = (double)*value;
}

void EnterValueWindow::setIncrementMode(IncrementMode mode) {
  if (mode == IncrementMode::COARSE) {
    minorIncrease = 200;
    majorIncrease = 2000;
    minorIncrement = 5;
    majorIncrement = 5;

  }else{
    minorIncrease = 200;
    majorIncrease = 3100;
    minorIncrement = 0.5;
    majorIncrement = 1;
  }
}
