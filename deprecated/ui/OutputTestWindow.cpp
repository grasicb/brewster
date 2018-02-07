#include "OutputTestWindow.h"
#include "../util/BrewsterGlobals.h"
#include "../controller/BrewsterController.h"

void OutputTestWindow::initScreen() {
  processChangeValue();
  tft->fillScreen(colorBackground);

  //Draw rectangles with temperature information
  tft->fillRect(0, 10, 320, 175, colorComponentBackground);

  tft->setTextColor(ILI9341_BLACK, colorComponentBackground);
  tft->setFont(font_standard);
  tft->setTextSize(1);
  tft->setCursor(10, 35);
  tft->print("Controller outputs");

  //tft->setFont(NULL);
  //tft->setTextSize(2);
  for (int i = 0;i<OUTPUT_NUMBER; i++) {
    tft->setFont(font_standard);
    tft->setCursor(10, 70+(i*30));
    tft->print(OutputNames[i]);

    outputStatus[i] = controller->getOutput((ControllerOutput)i)->isActive();
    outputPID[i] = controller->getOutput((ControllerOutput)i)->isPID();
    outputValue[i] = controller->getOutput((ControllerOutput)i)->getOutput();

//    outputSetValue[i] = 100;
//    outputSetPoint[i] = 28;

    tft->setFont(font_small);
    tft->setCursor(70, 70+(i*30));
    if (outputStatus[i]) {
      tft->print("ON");
    }else {
      tft->print("OFF");
    }

    tft->setFont(font_small);
    tft->setCursor(115, 70+(i*30));
    if(outputPID[i]) {
      tft->print("auto");
    }
  }
  tft->setTextColor(ILI9341_WHITE);
  tft->setFont(font_small);
  tft->setCursor(10, 230);
  tft->printf("Temp: ");

  lastMinute = 100;
  lastTemp = -100;

  //Define and draw buttons
  buttons.clear();
  buttons.push_back( Button(tft, 270, 190, 50, 50, icon_menu_bits, icon_menu_width, icon_menu_height, Action::W_MAIN_MENU));

  buttons.push_back( Button(tft, 170, 42, 40, 26, "s", Action::SETTINGS_0));
  buttons.push_back( Button(tft, 170, 72, 40, 26, "s", Action::SETTINGS_1));
  buttons.push_back( Button(tft, 170, 102, 40, 26, "s", Action::SETTINGS_2));
  buttons.push_back( Button(tft, 170, 132, 40, 26, "s", Action::SETTINGS_3));

  buttons.push_back( Button(tft, 220, 42, 40, 26, "m", Action::MODE_0));
  buttons.push_back( Button(tft, 220, 72, 40, 26, "m", Action::MODE_1));
  buttons.push_back( Button(tft, 220, 102, 40, 26, "m", Action::MODE_2));
  buttons.push_back( Button(tft, 220, 132, 40, 26, "m", Action::MODE_3));

  buttons.push_back( Button(tft, 270, 42, 40, 26, "o", Action::TOGGLE_0));
  buttons.push_back( Button(tft, 270, 72, 40, 26, "o", Action::TOGGLE_1));
  buttons.push_back( Button(tft, 270, 102, 40, 26, "o", Action::TOGGLE_2));
  buttons.push_back( Button(tft, 270, 132, 40, 26, "o", Action::TOGGLE_3));
  drawButtons();
}

void OutputTestWindow::refreshScreen() {
  tft->setTextColor(colorFont);
  tft->setFont(font_small);
  tft->setTextSize(1);

  // Refresing output parameters
  for (int i=0; i<OUTPUT_NUMBER; i++) {
    bool isActive = controller->getOutput((ControllerOutput)i)->isActive();
    //bool isPID = controller->getOutput((ControllerOutput)i)->isPID();
    //int output = controller->getOutput((ControllerOutput)i)->getOutput();

    if (isActive != outputStatus[i]) {
      Log.trace("Changing status for output %i to %i", i, isActive);
      outputStatus[i] = isActive;

      tft->fillRect(70, 54+(i*30), 40, 17, colorComponentBackground);
      tft->setCursor(70, 70+(i*30));
      if (isActive) {
        tft->print("ON");
      }else {
        tft->print("OFF");
      }
    }
  }

  //Refreshing temperature
  if (lastTemp != BrewsterController::get()->temperatureSensors->temperature[pidInputId]) {
    lastTemp = BrewsterController::get()->temperatureSensors->temperature[pidInputId];
    tft->setTextColor(ILI9341_WHITE);
    tft->setFont(font_standard);
    tft->setCursor(75, 230);
    tft->fillRect(75, 212, 70, 20, ILI9341_BLACK);
    tft->printf("%5.2f", lastTemp);
  }
}

void OutputTestWindow::process(void* param) {
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

  if ((lastRefresh + refreshRate) < millis()) {
    refreshScreen();
    lastRefresh = millis();
  }
}

void OutputTestWindow::processAction(uint8_t action) {
  Log.trace("Triggering action %03d in window OutputTestWindow", action);
  bool toggle = false;
  bool mode = false;
  bool settings = false;
  int outputID = 0;

  switch (action) {
    case Action::W_MAIN_MENU:
      windowManager->openWindow(WindowManager::Windows::WINDOW_SETTINGS);
      break;
    case Action::TOGGLE_0:
      toggle = true;
      outputID = 0;
      break;
    case Action::TOGGLE_1:
      toggle = true;
      outputID = 1;
      break;
    case Action::TOGGLE_2:
      toggle = true;
      outputID = 2;
      break;
    case Action::TOGGLE_3:
      toggle = true;
      outputID = 3;
      break;
    case Action::MODE_0:
      mode = true;
      outputID = 0;
      break;
    case Action::MODE_1:
      mode = true;
      outputID = 1;
      break;
    case Action::MODE_2:
      mode = true;
      outputID = 2;
      break;
    case Action::MODE_3:
      mode = true;
      outputID = 3;
      break;
    case Action::SETTINGS_0:
      settings = true;
      outputID = 0;
      break;
    case Action::SETTINGS_1:
      settings = true;
      outputID = 1;
      break;
    case Action::SETTINGS_2:
      settings = true;
      outputID = 2;
      break;
    case Action::SETTINGS_3:
      settings = true;
      outputID = 3;
      break;
    default:
      Log.warn("Action %03d not implemented in window OutputTestWindow", action);
      break;
  }

  if (toggle) {

    //If it is output without PID it sets a defined output rate
    if(!outputPID[outputID]) {
      uint8_t value = outputSetValue[outputID];
      if (controller->getOutput((ControllerOutput)outputID)->isActive() == true) {
        value = 0;
      }
      controller->getOutput((ControllerOutput)outputID)->setOutput(value);


    //If it is output with PID it sets the setpoint
    }else{
        if (controller->getOutput((ControllerOutput)outputID)->isActive() == true) {
          controller->getOutput((ControllerOutput)outputID)->setOutput(0);
        }else {
          controller->getOutput((ControllerOutput)outputID)->setTargetValue(outputSetPoint[outputID], &BrewsterController::get()->temperatureSensors->temperature[pidInputId]);
        }
    }
  }

  if (mode) {
    outputPID[outputID] = !outputPID[outputID];
    if(outputPID[outputID]) {
      tft->setFont(font_small);
      tft->setCursor(115, 70+(outputID*30));
      tft->print("auto");
    }else{
      tft->fillRect(115, 54+(outputID*30), 45, 17, colorComponentBackground);
    }
  }

  if (settings) {
    EnterValueWindow *enterValueWindow = (EnterValueWindow *) windowManager->getWindow(WindowManager::Windows::ENTER_VALUE);
    if (!outputPID[outputID]) {
      enterValueWindow->setLimits(0, 90);
      enterValueWindow->setIncrementMode(EnterValueWindow::IncrementMode::COARSE);
      enterValueWindow->setOutputValue(&outputSetValue[outputID]);
    }else{
      enterValueWindow->setLimits(0, 100);
      enterValueWindow->setIncrementMode(EnterValueWindow::IncrementMode::PRECISE);
      enterValueWindow->setOutputValue(&outputSetPoint[outputID]);
    }
    changeValueOutputID = outputID;
    windowManager->openWindow(enterValueWindow);
  }
}

void OutputTestWindow::processChangeValue() {
  if (changeValueOutputID >=0) {
    if (controller->getOutput((ControllerOutput)changeValueOutputID)->isActive()) {
      if (!outputPID[changeValueOutputID]) {
        controller->getOutput((ControllerOutput)changeValueOutputID)->setOutput(outputSetValue[changeValueOutputID]);
      }else{
        controller->getOutput((ControllerOutput)changeValueOutputID)->changeTargetValue(outputSetPoint[changeValueOutputID]);
      }
    }

    changeValueOutputID = -1;
  }
}
