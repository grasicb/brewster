#include "OutputTestController.h"
#include "UIEvent.h"
#include "../controller/BrewsterController.h"
#include "../controller/SensorManager.h"
#include "../controller/TemperatureSensor.h"


///////////////////////////
//Inherited functions
///////////////////////////

OutputTestController::OutputTestController() {
  logger = new Logger("OutputTestController");

  //Init sensors
  sensors = &(BrewsterController::get()->getSensorManager()->getAllTemperatureSensors());
  it=sensors->begin();
  selectedSensor = it->first;

  //Register callback functions
  bO1PowerSwitch.attachPop(cbPowerSwitchTriggered, new UIEvent(this, &bO1PowerSwitch));
  bO2PowerSwitch.attachPop(cbPowerSwitchTriggered, new UIEvent(this, &bO2PowerSwitch));
  bO3PowerSwitch.attachPop(cbPowerSwitchTriggered, new UIEvent(this, &bO3PowerSwitch));
  bO4PowerSwitch.attachPop(cbPowerSwitchTriggered, new UIEvent(this, &bO4PowerSwitch));
  bO1AutoSwitch.attachPop(cbAutoSwitchTriggered, new UIEvent(this, &bO1AutoSwitch));
  bO2AutoSwitch.attachPop(cbAutoSwitchTriggered, new UIEvent(this, &bO2AutoSwitch));
  bO3AutoSwitch.attachPop(cbAutoSwitchTriggered, new UIEvent(this, &bO3AutoSwitch));
  bO4AutoSwitch.attachPop(cbAutoSwitchTriggered, new UIEvent(this, &bO4AutoSwitch));
  bO1Settings.attachPop(cbSettingsTriggered, new UIEvent(this, &bO1Settings));
  bO2Settings.attachPop(cbSettingsTriggered, new UIEvent(this, &bO2Settings));
  bO3Settings.attachPop(cbSettingsTriggered, new UIEvent(this, &bO3Settings));
  bO4Settings.attachPop(cbSettingsTriggered, new UIEvent(this, &bO4Settings));
  bNextLocation.attachPop(cbNextLocation, new UIEvent(this, &bNextLocation));

  //Update listeners
  listenerList = new NexTouch*[14];
  listenerList[0] = &bO1PowerSwitch;
  listenerList[1] = &bO2PowerSwitch;
  listenerList[2] = &bO3PowerSwitch;
  listenerList[3] = &bO4PowerSwitch;
  listenerList[4] = &bO1AutoSwitch;
  listenerList[5] = &bO2AutoSwitch;
  listenerList[6] = &bO3AutoSwitch;
  listenerList[7] = &bO4AutoSwitch;
  listenerList[8] = &bO1Settings;
  listenerList[9] = &bO2Settings;
  listenerList[10] = &bO3Settings;
  listenerList[11] = &bO4Settings;
  listenerList[12] = &bNextLocation;
  listenerList[13] = NULL;
}

void OutputTestController::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);

  refreshScreen();
}

void OutputTestController::deactivateScreen() {
}

void OutputTestController::process() {
  //Update temperature if needed
  if (temperature != it->second.getValue())
    updateTempSensorValue();
}

///////////////////////////
//Business functions
///////////////////////////

void OutputTestController::refreshScreen() {
  //BrewsterController::get()

  for(int i=0; i<OUTPUT_NUMBER; i++) {
    Output *o = BrewsterController::get()->getOutput((ControllerOutput)i);
    NexDSButton *pwrBtn = NULL;
    NexDSButton *atBtn = NULL;
    NexNumber *valNum = NULL;

    if(i==0) {
      pwrBtn = &bO1PowerSwitch;
      atBtn = &bO1AutoSwitch;
      valNum = &nO1Value;
    } else if (i==1) {
      pwrBtn = &bO2PowerSwitch;
      atBtn = &bO2AutoSwitch;
      valNum = &nO2Value;
    } else if (i==2) {
      pwrBtn = &bO3PowerSwitch;
      atBtn = &bO3AutoSwitch;
      valNum = &nO3Value;
    } else if (i==3) {
      pwrBtn = &bO4PowerSwitch;
      atBtn = &bO4AutoSwitch;
      valNum = &nO4Value;
    } else {
      pwrBtn = NULL;
      atBtn = NULL;
      valNum = NULL;
      logger->error("OutputTestController::refreshScreen: Output number %i is not mapped.", i);
    }


    //Setting state of the power switch
    if(pwrBtn != NULL)
      pwrBtn->setValue(static_cast<uint32_t>(o->isActive()));

    //Setting state of the auto switch
    if(atBtn != NULL)
      atBtn->setValue(static_cast<uint32_t>(o->isPID()));

    //Setting value of the out (in case of PID this is target value, in case of standard output it is percentage turned on)
    if(valNum != NULL) {
      if(o->isPID())
        valNum->setValue(static_cast<uint32_t>(o->getTargetValue()));
      else
        valNum->setValue(static_cast<uint32_t>(o->getOutput()));
    }else
      valNum->setValue(-1);
  }

  updateTempSensorLocation();
}

void OutputTestController::updateTempSensorLocation() {
  updateTempSensorValue();
  tLocation.setText(sensorNames[selectedSensor]);
}

void OutputTestController::updateTempSensorValue() {
  temperature = it->second.getValue();
  nTemperature.setValue(static_cast<uint32_t>(temperature));
}

void OutputTestController::nextSensor() {
  if (it == sensors->end())
    it = sensors->begin();
  else
    it++;

  selectedSensor = it->first;
  updateTempSensorLocation();
}

///////////////////////////
//UI Callback functions
///////////////////////////

void OutputTestController::cbNextLocation(void *ptr) {
  UIEvent *obj = (UIEvent *) ptr;
  NexButton *button = (NexButton *)obj->getButton();
  OutputTestController *window = (OutputTestController *) obj->getWindowController();

  if (Log.isTraceEnabled())
    window->logger->trace("Next location button triggered");

  window->nextSensor();
}

void OutputTestController::cbPowerSwitchTriggered(void *ptr) {
  UIEvent *obj = (UIEvent *) ptr;
  NexDSButton *button = (NexDSButton *)obj->getButton();
  OutputTestController *w = (OutputTestController *) obj->getWindowController();

  if (Log.isTraceEnabled())
    w->logger->trace("Power switch triggered (button: %s)", button->getObjName());

  Output *o = NULL;
  NexDSButton *pwrBtn = NULL;
  NexDSButton *atBtn = NULL;
  NexNumber *valNum = NULL;

  //Get the correct references to output and buttons based on which button has been pressed
  if(button = &w->bO1PowerSwitch) {
    pwrBtn = &w->bO1PowerSwitch;
    atBtn = &w->bO1AutoSwitch;
    valNum = &w->nO1Value;
    o = BrewsterController::get()->getOutput(ControllerOutput::AC1);
  } else if (button = &w->bO2PowerSwitch) {
    pwrBtn = &w->bO2PowerSwitch;
    atBtn = &w->bO2AutoSwitch;
    valNum = &w->nO2Value;
    o = BrewsterController::get()->getOutput(ControllerOutput::AC2);
  } else if (button = &w->bO3PowerSwitch) {
    pwrBtn = &w->bO3PowerSwitch;
    atBtn = &w->bO3AutoSwitch;
    valNum = &w->nO3Value;
    o = BrewsterController::get()->getOutput(ControllerOutput::DC1);
  } else if (button = &w->bO4PowerSwitch) {
    pwrBtn = &w->bO4PowerSwitch;
    atBtn = &w->bO4AutoSwitch;
    valNum = &w->nO4Value;
    o = BrewsterController::get()->getOutput(ControllerOutput::DC2);
  } else {
    pwrBtn = NULL;
    atBtn = NULL;
    valNum = NULL;
    o = NULL;
    w->logger->error("cbPowerSwitchTriggered: Button ""%s"" is not mapped.", button->getObjName());
  }

  //////////////////
  // Business logic
  if(pwrBtn != NULL && o != NULL) {
    uint32_t swPower;
    uint32_t swAuto;
    uint32_t value;

    pwrBtn->getValue(&swPower);
    atBtn->getValue(&swAuto);
    valNum->getValue(&value);

    ///////////////////////////
    //Turn off selected output
    if(swPower == 0) {
      o->setOutput(0);


    ///////////////////////////
    //Turn on selected output
    }else{
      //If pid is enabled, then turn on PID
      if(swAuto == 1) {
        o->setTargetValue(value, w->it->second.getValueReference());

      //If it is the classical output just turn on the output
      }else{
        o->setOutput(value);
      }
    }
  }
}

void OutputTestController::cbAutoSwitchTriggered(void *ptr) {
  UIEvent *obj = (UIEvent *) ptr;
  NexDSButton *button = (NexDSButton *)obj->getButton();
  OutputTestController *w = (OutputTestController *) obj->getWindowController();

  if (Log.isTraceEnabled())
    w->logger->trace("Auto switch triggered (button: %s)", button->getObjName());

  NexDSButton *pwrBtn = NULL;
  NexDSButton *atBtn = NULL;

  //////////////////
  //Get the correct references to output and buttons based on which button has been pressed
  if(button = &w->bO1AutoSwitch) {
    pwrBtn = &w->bO1PowerSwitch;
    atBtn = &w->bO1AutoSwitch;
  } else if (button = &w->bO2AutoSwitch) {
    pwrBtn = &w->bO2PowerSwitch;
    atBtn = &w->bO2AutoSwitch;
  } else if (button = &w->bO3AutoSwitch) {
    pwrBtn = &w->bO3PowerSwitch;
    atBtn = &w->bO3AutoSwitch;
  } else if (button = &w->bO4AutoSwitch) {
    pwrBtn = &w->bO4PowerSwitch;
    atBtn = &w->bO4AutoSwitch;
  } else {
    pwrBtn = NULL;
    atBtn = NULL;
    w->logger->error("cbAutoSwitchTriggered: Button ""%s"" is not mapped.", button->getObjName());
  }

  //////////////////
  // Business logic
  if (pwrBtn != NULL && atBtn != NULL) {
    uint32_t swPower;
    uint32_t swAuto;
    pwrBtn->getValue(&swPower);
    atBtn->getValue(&swAuto);

    if(swPower == 1) {
      w->logger->warn("cbAutoSwitchTriggered: Changing auto change mode is possible only if output is turned off");
      if(swAuto == 0)
        atBtn->setValue(1);
      else
        atBtn->setValue(0);
    }
  }
}

void OutputTestController::cbSettingsTriggered(void *ptr) {
  UIEvent *obj = (UIEvent *) ptr;
  NexDSButton *button = (NexDSButton *)obj->getButton();
  OutputTestController *w = (OutputTestController *) obj->getWindowController();

  if (Log.isTraceEnabled())
    w->logger->trace("Settings triggered (button: %s)", button->getObjName());

  Output *o = NULL;
  NexDSButton *pwrBtn = NULL;
  NexDSButton *atBtn = NULL;
  NexNumber *valNum = NULL;

  //////////////////
  //Get the correct references to output and buttons based on which button has been pressed
  if(button = &w->bO1Settings) {
    pwrBtn = &w->bO1PowerSwitch;
    atBtn = &w->bO1AutoSwitch;
    valNum = &w->nO1Value;
    o = BrewsterController::get()->getOutput(ControllerOutput::AC1);
  } else if (button = &w->bO2Settings) {
    pwrBtn = &w->bO2PowerSwitch;
    atBtn = &w->bO2AutoSwitch;
    valNum = &w->nO2Value;
    o = BrewsterController::get()->getOutput(ControllerOutput::AC2);
  } else if (button = &w->bO3Settings) {
    pwrBtn = &w->bO3PowerSwitch;
    atBtn = &w->bO3AutoSwitch;
    valNum = &w->nO3Value;
    o = BrewsterController::get()->getOutput(ControllerOutput::DC1);
  } else if (button = &w->bO4Settings) {
    pwrBtn = &w->bO4PowerSwitch;
    atBtn = &w->bO4AutoSwitch;
    valNum = &w->nO4Value;
    o = BrewsterController::get()->getOutput(ControllerOutput::DC2);
  } else {
    pwrBtn = NULL;
    atBtn = NULL;
    valNum = NULL;
    o = NULL;
    w->logger->error("cbSettingsTriggered: Button ""%s"" is not mapped.", button->getObjName());
  }

  //////////////////
  // Business logic
  if(valNum != NULL && o != NULL && o->isActive()) {
    uint32_t value;
    valNum->getValue(&value);

    if(o->isPID()) {
      o->changeTargetValue(value);
    }else{
      o->setOutput(value);
    }
  }
}

//Brewster Callback functions
void OutputTestController::outputChangedEvent(void* callingObject, void* outputIdentifier, OutputChangeEvent event) {

}
