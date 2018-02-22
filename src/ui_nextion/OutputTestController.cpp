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

  //Register callback functions for UI Events
  for(int i=0; i<arraySize(bPowerSwitches); i++) {
    bPowerSwitches[i].attachPop(cbPowerSwitchTriggered, new UIEvent(this, &bPowerSwitches[i]));
    bAutoSwitches[i].attachPop(cbAutoSwitchTriggered, new UIEvent(this, &bAutoSwitches[i]));
    bSettings[i].attachPop(cbSettingsTriggered, new UIEvent(this, &bSettings[i]));
  }
  bNextLocation.attachPop(cbNextLocation, new UIEvent(this, &bNextLocation));

  //Update listeners for UI Events
  int j=0;
  listenerList = new NexTouch*[(arraySize(bPowerSwitches)*3)+2];
  for(int i=0; i<arraySize(bPowerSwitches); i++) {
    listenerList[j++] = &bPowerSwitches[i];
    listenerList[j++] = &bAutoSwitches[i];
    listenerList[j++] = &bSettings[i];
  }
  listenerList[j++] = &bNextLocation;
  listenerList[j++] = NULL;

  //Register callback for output changes
  logger->info("Registering output listeners for OutputTestController");
  for (int i=0; i<OUTPUT_NUMBER; i++) {
    Output *o = BrewsterController::get()->getOutput((ControllerOutput)i);
    o->addListener(outputChangedEvent, this, i);
  }
}

void OutputTestController::initializeScreen(void *ptr) {
  AWindowController::initializeScreen(ptr);

  refreshScreen();
}

void OutputTestController::deactivateScreen() {
  //Register callback for output changes
  logger->info("Deregistering output listeners for OutputTestController");
  for (int i=0; i<OUTPUT_NUMBER; i++) {
    Output *o = BrewsterController::get()->getOutput((ControllerOutput)i);
    o->removeListener(outputChangedEvent);
  }
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

  for(int i=0; i<arraySize(bPowerSwitches); i++) {
    Output *o = BrewsterController::get()->getOutput((ControllerOutput)i);

<<<<<<< HEAD
    //Setting value of the out (in case of PID this is target value, in case of standard output it is percentage turned on)
    if(valNum != NULL) {
      if(o->isPID()) {
        valNum->setValue(static_cast<uint32_t>(o->getTargetValue()));
      }else{
        valNum->setValue(static_cast<uint32_t>(o->getOutput()));
      }
    }else
      valNum->setValue(-1);
=======
    bPowerSwitches[i].setValue(static_cast<uint32_t>(o->isActive()));
    bAutoSwitches[i].setValue(static_cast<uint32_t>(o->isPID()));
    if(o->isPID())
      nValues[i].setValue(static_cast<uint32_t>(o->getTargetValue()));
    else
      nValues[i].setValue(static_cast<uint32_t>(o->getOutput()));
>>>>>>> 9164678963c93486f46a05d84a3f71612b810ceb
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

  ///////////////////
  // Identifying the output associated to the button
  Output *o = NULL;
<<<<<<< HEAD
  NexDSButton *pwrBtn = NULL;
  NexDSButton *atBtn = NULL;
  NexNumber *valNum = NULL;

  //Get the correct references to output and buttons based on which button has been pressed
  if(button == &w->bO1PowerSwitch) {
    pwrBtn = &w->bO1PowerSwitch;
    atBtn = &w->bO1AutoSwitch;
    valNum = &w->nO1Value;
    o = BrewsterController::get()->getOutput(ControllerOutput::AC1);
  } else if (button == &w->bO2PowerSwitch) {
    pwrBtn = &w->bO2PowerSwitch;
    atBtn = &w->bO2AutoSwitch;
    valNum = &w->nO2Value;
    o = BrewsterController::get()->getOutput(ControllerOutput::AC2);
  } else if (button == &w->bO3PowerSwitch) {
    pwrBtn = &w->bO3PowerSwitch;
    atBtn = &w->bO3AutoSwitch;
    valNum = &w->nO3Value;
    o = BrewsterController::get()->getOutput(ControllerOutput::DC1);
  } else if (button == &w->bO4PowerSwitch) {
    pwrBtn = &w->bO4PowerSwitch;
    atBtn = &w->bO4AutoSwitch;
    valNum = &w->nO4Value;
    o = BrewsterController::get()->getOutput(ControllerOutput::DC2);
  } else {
    pwrBtn = NULL;
    atBtn = NULL;
    valNum = NULL;
    o = NULL;
=======
  int i = -1;

  for(int j=0; j<arraySize(bPowerSwitches); j++) {
    if(button == &w->bPowerSwitches[j]) {
      i = j;
      o = BrewsterController::get()->getOutput((ControllerOutput)i);
      break;
    }
  }

  if(i == -1) {
>>>>>>> 9164678963c93486f46a05d84a3f71612b810ceb
    w->logger->error("cbPowerSwitchTriggered: Button ""%s"" is not mapped.", button->getObjName());
    return;
  }

  //////////////////
  // Business logic
  uint32_t swPower;
  uint32_t swAuto;
  uint32_t value;

  w->bPowerSwitches[i].getValue(&swPower);
  w->bAutoSwitches[i].getValue(&swAuto);
  w->nValues[i].getValue(&value);

  //Turn off selected output
  if(swPower == 0) {
    o->setOutput(0);

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

void OutputTestController::cbAutoSwitchTriggered(void *ptr) {
  UIEvent *obj = (UIEvent *) ptr;
  NexDSButton *button = (NexDSButton *)obj->getButton();
  OutputTestController *w = (OutputTestController *) obj->getWindowController();

  if (Log.isTraceEnabled())
    w->logger->trace("Auto switch triggered (button: %s)", button->getObjName());

  ///////////////////
  // Identifying the output associated to the button
  int i = -1;

<<<<<<< HEAD
  //////////////////
  //Get the correct references to output and buttons based on which button has been pressed
  if(button == &w->bO1AutoSwitch) {
    pwrBtn = &w->bO1PowerSwitch;
    atBtn = &w->bO1AutoSwitch;
  } else if (button == &w->bO2AutoSwitch) {
    pwrBtn = &w->bO2PowerSwitch;
    atBtn = &w->bO2AutoSwitch;
  } else if (button == &w->bO3AutoSwitch) {
    pwrBtn = &w->bO3PowerSwitch;
    atBtn = &w->bO3AutoSwitch;
  } else if (button == &w->bO4AutoSwitch) {
    pwrBtn = &w->bO4PowerSwitch;
    atBtn = &w->bO4AutoSwitch;
  } else {
    pwrBtn = NULL;
    atBtn = NULL;
=======
  for(int j=0; j<arraySize(bPowerSwitches); j++) {
    if(button == &w->bAutoSwitches[j]) {
      i = j;
      break;
    }
  }

  if(i == -1) {
>>>>>>> 9164678963c93486f46a05d84a3f71612b810ceb
    w->logger->error("cbAutoSwitchTriggered: Button ""%s"" is not mapped.", button->getObjName());
    return;
  }

  //////////////////
  // Business logic
  uint32_t swPower;
  uint32_t swAuto;
  w->bPowerSwitches[i].getValue(&swPower);
  w->bAutoSwitches[i].getValue(&swAuto);

  if(swPower == 1) {
    w->logger->warn("cbAutoSwitchTriggered: Changing auto change mode is possible only if output is turned off");
    if(swAuto == 0)
      w->bAutoSwitches[i].setValue(1);
    else
      w->bAutoSwitches[i].setValue(0);
  }
}

void OutputTestController::cbSettingsTriggered(void *ptr) {
  UIEvent *obj = (UIEvent *) ptr;
  NexDSButton *button = (NexDSButton *)obj->getButton();
  OutputTestController *w = (OutputTestController *) obj->getWindowController();

  if (Log.isTraceEnabled())
    w->logger->trace("Settings triggered (button: %s)", button->getObjName());

  ///////////////////
  // Identifying the output associated to the button
  Output *o = NULL;
  int i = -1;

<<<<<<< HEAD
  //////////////////
  //Get the correct references to output and buttons based on which button has been pressed
  if(button == &w->bO1Settings) {
    pwrBtn = &w->bO1PowerSwitch;
    atBtn = &w->bO1AutoSwitch;
    valNum = &w->nO1Value;
    o = BrewsterController::get()->getOutput(ControllerOutput::AC1);
  } else if (button == &w->bO2Settings) {
    pwrBtn = &w->bO2PowerSwitch;
    atBtn = &w->bO2AutoSwitch;
    valNum = &w->nO2Value;
    o = BrewsterController::get()->getOutput(ControllerOutput::AC2);
  } else if (button == &w->bO3Settings) {
    pwrBtn = &w->bO3PowerSwitch;
    atBtn = &w->bO3AutoSwitch;
    valNum = &w->nO3Value;
    o = BrewsterController::get()->getOutput(ControllerOutput::DC1);
  } else if (button == &w->bO4Settings) {
    pwrBtn = &w->bO4PowerSwitch;
    atBtn = &w->bO4AutoSwitch;
    valNum = &w->nO4Value;
    o = BrewsterController::get()->getOutput(ControllerOutput::DC2);
  } else {
    pwrBtn = NULL;
    atBtn = NULL;
    valNum = NULL;
    o = NULL;
=======
  for(int j=0; j<arraySize(bPowerSwitches); j++) {
    if(button == &w->bSettings[j]) {
      i = j;
      o = BrewsterController::get()->getOutput((ControllerOutput)i);
      break;
    }
  }

  if(i == -1) {
>>>>>>> 9164678963c93486f46a05d84a3f71612b810ceb
    w->logger->error("cbSettingsTriggered: Button ""%s"" is not mapped.", button->getObjName());
    return;
  }

  //////////////////
  // Business logic
  if(o->isActive()) {
    uint32_t value;
    w->nValues[i].getValue(&value);

    if(o->isPID()) {
      o->changeTargetValue(value);
    }else{
      o->setOutput(value);
    }
  }
}

/////////////////////////////////
// Output Callback functions
/////////////////////////////////
void OutputTestController::outputChangedEvent(void* callingObject, int outputIdentifier, OutputChangeEvent event) {
  OutputTestController *w = (OutputTestController *) callingObject;

  w->logger->info("Output event change received for output %i [ON=%i, AUTO=%i, VALUE=%.1f]", outputIdentifier, (int)event.isActive, (int)event.isPID, event.targetValue);

  w->bPowerSwitches[outputIdentifier].setValue((int)event.isActive);
  w->bAutoSwitches[outputIdentifier].setValue((int)event.isPID);
  w->nValues[outputIdentifier].setValue((int)event.targetValue);
}
