#include "Process.h"
#include "../util/BrewsterUtils.h"

Process::Process(BrewProcess type, String name) {
    logger = new Logger("Process");
    this->type = type;
    this->name = name;
    loadFromEEPROM();
}

void Process::start() {
  ProcessStateChangeEvent event;
  event.oldState = state;

  //Business logic
  if(state == ProcessState::STARTED)
    logger->error("Cannot start process %s. Process already started.", (const char*) name);
  else if(state == ProcessState::PAUSED)
    logger->error("Cannot start process %s. Process is paused.", (const char*) name);
  else {
    startTime = Time.now();
    state = ProcessState::STARTED;
    storeToEEPROM();
  }

  //Triggering state change events
  event.newState = state;
  event.process = this;
  if (event.oldState != event.newState)
    triggerStateChangeEvent(event);
}

void Process::stop() {
  ProcessStateChangeEvent event;
  event.oldState = state;

  //Business logic
  if(state == ProcessState::STOPPED)
    logger->warn("Process %s was not running. Trying to stop not running process.", (const char*) name);

  startTime = 0;
  state = ProcessState::STOPPED;
  storeToEEPROM();

  //Triggering state change events
  event.newState = state;
  event.process = this;
  if (event.oldState != event.newState)
    triggerStateChangeEvent(event);
}

void Process::pause() {
  ProcessStateChangeEvent event;
  event.oldState = state;

  //Business logic
  if(state == ProcessState::STOPPED)
    logger->error("Cannot pause process %s. The process is stopped.", (const char*) name);
  else if(state == ProcessState::PAUSED)
    logger->warn("The process %s is already paused. Trying to pause a paused process.", (const char*) name);

  else {
    state = ProcessState::PAUSED;
    storeToEEPROM();
  }

  //Triggering state change events
  event.newState = state;
  event.process = this;
  if (event.oldState != event.newState)
    triggerStateChangeEvent(event);
}

void Process::resume() {
  ProcessStateChangeEvent event;
  event.oldState = state;

  //Business logic
  if(state == ProcessState::STOPPED)
    logger->error("Cannot resume process %s. The process is stopped.", (const char*) name);
  else if(state == ProcessState::PAUSED)
    logger->warn("The process %s is already running. Trying to resume a running process.", (const char*) name);

  else {
    state = ProcessState::STARTED;
    storeToEEPROM();
  }

  //Triggering state change events
  event.newState = state;
  event.process = this;
  if (event.oldState != event.newState)
    triggerStateChangeEvent(event);
}

void Process::storeToEEPROM() {
  EEPROM.put(EEPROM_PROCESS_DATA[type], (uint8_t) 1);
  EEPROM.put(EEPROM_PROCESS_DATA[type]+1, startTime);
  EEPROM.put(EEPROM_PROCESS_DATA[type]+5, state);
}

void Process::loadFromEEPROM() {
  uint8_t dataAvailable = 0;
  EEPROM.get(EEPROM_PROCESS_DATA[type], dataAvailable);

  if(dataAvailable == 1) {
    EEPROM.get(EEPROM_PROCESS_DATA[type]+1, startTime);
    EEPROM.get(EEPROM_PROCESS_DATA[type]+5, state);
  }else{
    state = ProcessState::STOPPED;
    startTime = 0;
  }
}

unsigned long Process::getStartTime() {
    return startTime;
}

unsigned long Process::getRunTime() {
  return Time.now() - startTime;
}

unsigned long Process::getRunTime(TimeUOM timeUOM) {
  return BrewsterUtils::convertSeconds(getRunTime(), timeUOM);
}

BrewProcess Process::getType() {
  return type;
}

String Process::getName() {
  return name;
}

ProcessState Process::getState() {
  return state;
}

///////////////////////////
// Event handling
///////////////////////////
void Process::addListener(f_processStateChange_t function, void* callingObject) {
  StateChageListener listener;
  listener.function = function;
  listener.callingObject = callingObject;

  listeners[function] = listener;

  logger->trace("Added new listener. Total listeners: %i.", listeners.size());
}

void Process::removeListener(f_processStateChange_t function) {
  int removed = listeners.erase(function);

  logger->trace("Removed %i listener(s). Total listeners after removal: %i.", removed, listeners.size());
}

void Process::removeAllListeners() {
  listeners.clear();

  logger->trace("Removed all listeners.");
}

void Process::triggerStateChangeEvent(ProcessStateChangeEvent& event)  {
  if(event.newState == ProcessState::STARTED && event.oldState == ProcessState::STOPPED)
    processStarted();
  else if(event.newState == ProcessState::STARTED && event.oldState == ProcessState::PAUSED)
    processResumed();
  else if(event.newState == ProcessState::STOPPED)
    processStopped();
  else if(event.newState == ProcessState::PAUSED)
    processPaused();

  for (std::map<f_processStateChange_t, StateChageListener>::iterator it=listeners.begin(); it!=listeners.end(); ++it)
    it->first(it->second.callingObject, event);
}
