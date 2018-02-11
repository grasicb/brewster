#include "TempUtils.h"

void TempUtils::listSensors() {
  BrewsterGlobals::get()->i2cMutex.lock();

  OneWire wire = OneWire(BrewsterGlobals::get()->pinOneWire);

  uint8_t addr[8];
  uint8_t i = 0;

  Log.info("Listing sensors:");
  wire.wireResetSearch();

  //wire.search(addr);
  while (wire.search(addr)) {
    i++;

    String type;
    switch (addr[0]) {
      case 0x10:
        type = "DS1820/DS18S20";
        break;
      case 0x28:
        type = "DS18B20";
        break;
      case 0x22:
        type = "DS1822";
        break;
      case 0x26:
        type = "DS2438";
        break;
      default:
        type = "unknown";
    }

    Log.info("\tSensor %d: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X (type: %s)", i, addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], addr[6], addr[7], (const char*) type);
  }

  if (i == 0) {
    Log.info("No sensors found.");
  }else{
    Log.info("Done");
  }

  BrewsterGlobals::get()->i2cMutex.unlock();
}

void TempUtils::setPrecision(DS18::PRECISION p) {
  Log.info("Setting precision of all sensors.");
  BrewsterGlobals::get()->i2cMutex.lock();

  DS18 ds18Sensor(BrewsterGlobals::get()->pinOneWire);
  ds18Sensor.setPrecision(p);

  BrewsterGlobals::get()->i2cMutex.unlock();
}
/*
void TempUtils::readTemperature() {
  BrewsterGlobals::get()->i2cMutex.lock();

  DS18 ds18Sensor(BrewsterGlobals::get()->pinOneWire);

  for (int i=0; i<BrewsterGlobals::tempSensorsNo; i++) {
    if (ds18Sensor.read(BrewsterGlobals::get()->tempSensors[i])) {
  		Serial.printf("Sensor %d: %.1f\n", i, ds18Sensor.celsius());
  	}
  }

  BrewsterGlobals::get()->i2cMutex.unlock();
}
*/
