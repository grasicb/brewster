#include "Process.h"
#include "../util/BrewsterUtils.h"

Process::Process(BrewProcess type, String name) {
    logger = new Logger("Process");
    this->type = type;
    this->name = name;
    recipeMandatory = false;
}

Process::Process(BrewProcess type, String name, Recipe* recipe) :Process(type, name) {
    setRecipe(recipe);
}

void Process::start() {
  ProcessStateChangeEvent event;
  event.oldState = state;

  //Business logic
  if (recipeMandatory && recipe == NULL)
    logger->error("Recipe is mandatory for this process, but is not set. Cannot start the process.");
  else if(state == ProcessState::STARTED)
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
  if (recipeMandatory && recipe == NULL)
    logger->error("Recipe is mandatory for this process, but is not set. Cannot start the process.");
  else if(state == ProcessState::STOPPED)
    logger->error("Cannot resume process %s. The process is stopped.", (const char*) name);
  else if(state == ProcessState::STARTED)
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
  logger->trace("Saving process %s to EEPROM [state=%s, startTime=%u].", (const char*) name, (const char*)ProcessStateNames[state], startTime);
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

  logger->trace("Process %s is restored from EEPROM [state=%s, startTime=%u, mem location=%i].", (const char*)name, (const char*)ProcessStateNames[state], startTime, EEPROM_PROCESS_DATA[type]);
}

void Process::restoreProcess() {
  loadFromEEPROM();
  processRestored();
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

boolean Process::isActive() {
  if(state == ProcessState::STOPPED)
    return false;
  else
    return true;
}

void Process::setRecipe(Recipe *recipe) {
    this->recipe = recipe;
}

Recipe* Process::getRecipe() {
  return recipe;
}

///////////////////////////
// Event handling - State Chage
///////////////////////////
void Process::addListener(f_processStateChange_t function, void* callingObject) {
  StateChageListener listener;
  listener.function = function;
  listener.callingObject = callingObject;

  listenersStateChange[function] = listener;

  logger->trace("Added new state change listener. Total listeners: %i.", listenersStateChange.size());
}

void Process::removeListener(f_processStateChange_t function) {
  int removed = listenersStateChange.erase(function);

  logger->trace("Removed %i state change listener(s). Total listeners after removal: %i.", removed, listenersStateChange.size());
}

void Process::removeAllStateChangeListeners() {
  listenersStateChange.clear();

  logger->trace("Removed all state change listeners.");
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

  for (StateChangeListenerMap::iterator it=listenersStateChange.begin(); it!=listenersStateChange.end(); ++it)
    it->first(it->second.callingObject, event);
}


///////////////////////////
// Event handling - Info Chage
///////////////////////////
void Process::addListener(f_processInfoChange_t function, void* callingObject) {
  InfoChageListener listener;
  listener.function = function;
  listener.callingObject = callingObject;

  listenersInfoChange[function] = listener;

  logger->trace("Added new information change listener. Total listeners: %i.", listenersInfoChange.size());
}

void Process::removeListener(f_processInfoChange_t function) {
  int removed = listenersInfoChange.erase(function);

  logger->trace("Removed %i information change listener(s). Total listeners after removal: %i.", removed, listenersInfoChange.size());
}

void Process::removeAllListeners() {
  removeAllStateChangeListeners();
  removeAllInfoChangeListeners();
}

void Process::triggerInfoChangeEvent()  {
  for (InfoChangeListenerMap::iterator it=listenersInfoChange.begin(); it!=listenersInfoChange.end(); ++it)
    it->first(it->second.callingObject, this);
}

void Process::removeAllInfoChangeListeners() {
  listenersInfoChange.clear();

  logger->trace("Removed all information change listeners.");
}


void Process::setStartTime(long startTime) {
  this->startTime = startTime;
}
