#include "SensorManager.h"

SensorManager::SensorManager() {
    logger = new Logger("sensor_mgmt");
    tempSensorsNo = TemperatureSensor::sensorLocationSize;

    //temperatureSensors = new TemperatureSensor[tempSensorsNo];
    temperatureSensors = (TemperatureSensor *) malloc(sizeof(new TemperatureSensor(NULL, TemperatureSensor::SensorLocation::HLT))*tempSensorsNo);

    for (int i = 0; i<tempSensorsNo; i++) {
      temperatureSensors[i] = NULL;
    }

    temperatureSensors[TemperatureSensor::SensorLocation::HLT] = new TemperatureSensor({0x28, 0xFF, 0x60, 0xA0, 0x64, 0x16, 0x3, 0x6F}, TemperatureSensor::SensorLocation::HLT);
    temperatureSensors[TemperatureSensor::SensorLocation::MT] = new TemperatureSensor({0x28, 0xFF, 0xD0, 0x50, 0x63, 0x16, 0x4, 0xA8}, TemperatureSensor::SensorLocation::MT);
    temperatureSensors[TemperatureSensor::SensorLocation::BK] = new TemperatureSensor({0x28, 0xFF, 0x7D, 0x34, 0x63, 0x16, 0x3, 0x6A}, TemperatureSensor::SensorLocation::BK);
    temperatureSensors[TemperatureSensor::SensorLocation::COOLER_OUT] = new TemperatureSensor({0x28, 0xFF, 0xB7, 0xC3, 0x51, 0x17, 0x4, 0xD4}, TemperatureSensor::SensorLocation::COOLER_OUT);
    temperatureSensors[TemperatureSensor::SensorLocation::FERMENTOR] = new TemperatureSensor({0x28, 0xFF, 0xDF, 0x8E, 0xC1, 0x16, 0x04, 0x6A}, TemperatureSensor::SensorLocation::FERMENTOR);
}


void SensorManager::readSensors() {
  readTemperatureSensors();
}

void SensorManager::readTemperatureSensors() {
  for (int i = 0; i < tempSensorsNo; i++) {
    if (temperatureSensors[i] != NULL) {
      temperatureSensors[i]->readSensor();
    }
  }
}

TemperatureSensor *SensorManager::getTemperatureSensor(TemperatureSensor::SensorLocation sensorLocation) {
  return temperatureSensors[sensorLocation];
}

TemperatureSensor **SensorManager::getAllTemperatureSensors() {
  return temperatureSensors;
}

uint8_t SensorManager::getTemperatureSensorNumber() {
  return tempSensorsNo;
}
