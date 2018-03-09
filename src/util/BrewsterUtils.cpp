#include "BrewsterUtils.h"

unsigned long BrewsterUtils::getSeconds(unsigned long time, TimeUOM timeUOM) {
  switch (timeUOM) {
    case TimeUOM::second :
      return ((unsigned long)time);

    case TimeUOM::minute :
      return ((unsigned long)time)*60;

    case TimeUOM::hour :
      return ((unsigned long)time)*3600;

    case TimeUOM::day :
      return ((unsigned long)time)*86400;

    default:
      Log.error("BrewsterUtils::getSeconds: Missing conversion for TimeUOM enum %i", (int)timeUOM);
      return 0;
  }
}

unsigned long BrewsterUtils::convertSeconds(unsigned long timeSeconds, TimeUOM timeUOM) {
  switch (timeUOM) {
    case TimeUOM::second :
      return ((unsigned long)timeSeconds);

    case TimeUOM::minute :
      return ((unsigned long)timeSeconds)/60;

    case TimeUOM::hour :
      return ((unsigned long)timeSeconds)/3600;

    case TimeUOM::day :
      return ((unsigned long)timeSeconds)/86400;

    default:
      Log.error("BrewsterUtils::convertSeconds: Missing conversion for TimeUOM enum %i", (int)timeUOM);
      return 0;
  }
}

void BrewsterUtils::i2c_scanner() {
  BrewsterGlobals::get()->i2cMutex.lock();
  byte error, address;
  int nDevices;

  Log.info("Scanning I2C decives");

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }
  }
  if (nDevices == 0)
    Log.info("No I2C devices found");
  else
    Log.info("I2C scan complete");

  BrewsterGlobals::get()->i2cMutex.unlock();
}
