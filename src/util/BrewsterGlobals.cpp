#include "BrewsterGlobals.h"

BrewsterGlobals* BrewsterGlobals::instance = NULL;

BrewsterGlobals::BrewsterGlobals() {
  logger = new Logger("globals");

  loadPIDSettings();
}

BrewsterGlobals* BrewsterGlobals::get() {
  if (instance == NULL) {
    instance = new BrewsterGlobals();
  }

  return instance;
}

void BrewsterGlobals::loadPIDSettings() {
  logger->info("Loading PID settings");
  int location = EEPROM_PID_SETTINGS;
  uint8_t settingsAvailable=0xFF;
  //EEPROM.write(location, settingsAvailable);
  EEPROM.get(location++, settingsAvailable);

  if(settingsAvailable == 1) {
    logger->trace("Loading from EEPROM");
    for(int i=1; i<6; i++) {
      PidSettings ps;
      EEPROM.get(location, ps.kp);
      location += 8;
      EEPROM.get(location, ps.ki);
      location += 8;
      EEPROM.get(location, ps.kd);
      location += 8;

      pidSettings[(BrewProcess)i] = ps;
      logger->trace("Process %i: p=%.1f, i=%.1f, d=%.1f", i, ps.kp, ps.ki, ps.kd);
    }

  }else {
    logger->trace("Settings not stored. Setting to initial values.");
    for(int i=1; i<6; i++) {
      PidSettings ps;
      ps.kp = 2;
      ps.ki = 5;
      ps.kd = 1;

      pidSettings[(BrewProcess)i] = ps;
      logger->trace("Process %i: p=%.1f, i=%.1f, d=%.1f", i, ps.kp, ps.ki, ps.kd);
    }
  }
}

void BrewsterGlobals::storePIDSettings() {
  logger->info("Storing PID settings");
  int location = EEPROM_PID_SETTINGS;
  uint8_t settingsAvailable = 1;
  EEPROM.write(location++, settingsAvailable);

  for(int i=1; i<6; i++) {
    EEPROM.write(location, pidSettings[(BrewProcess)i].kp);
    location += 8;
    EEPROM.write(location, pidSettings[(BrewProcess)i].ki);
    location += 8;
    EEPROM.write(location, pidSettings[(BrewProcess)i].kd);
    location += 8;
  }
}

t_map_pidSettings& BrewsterGlobals::getPIDSettings() {
  return pidSettings;
}
