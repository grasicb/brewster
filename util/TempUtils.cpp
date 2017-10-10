#include "TempUtils.h"

void TempUtils::listSensors() {
  OneWire wire = OneWire(BrewsterGlobals::get()->pinOneWire);

  uint8_t addr[8];
  uint8_t i = 0;

  Log.info("Listing sensors:");

  while (wire.search(addr)) {
    i++;

    char *type;
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

    Log.info("\tSensor %d: 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X 0x%02X (type: %s)", i, addr[0], addr[1], addr[2], addr[3], addr[4], addr[5], addr[6], addr[7], type);
  }
}

void TempUtils::setPrecision(DS18::PRECISION p) {
  DS18 ds18Sensor(BrewsterGlobals::get()->pinOneWire);

  ds18Sensor.setPrecision(p);
}

void TempUtils::readTemperature() {
  DS18 ds18Sensor(BrewsterGlobals::get()->pinOneWire);

	if (ds18Sensor.read(BrewsterGlobals::get()->tempSensor1)) {
		Serial.printf("Sensor 1: %.1f\n", ds18Sensor.celsius());
	}
	if (ds18Sensor.read(BrewsterGlobals::get()->tempSensor2)) {
		Serial.printf("Sensor 2: %.1f\n", ds18Sensor.celsius());
	}
	if (ds18Sensor.read(BrewsterGlobals::get()->tempSensor3)) {
		Serial.printf("Sensor 3: %.1f\n", ds18Sensor.celsius());
	}
}
