#include "SensorManager.h"

SensorManager::SensorManager() {
    logger = new Logger("sensor_mgmt");
    //tempSensorsNo = TemperatureSensor::sensorLocationSize;

    //temperatureSensors = std::vector<TemperatureSensor> (tempSensorsNo);
    //temperatureSensors = new TemperatureSensor *[tempSensorsNo];
    //temperatureSensors = (TemperatureSensor **) malloc(sizeof(new TemperatureSensor(NULL, TemperatureSensor::SensorLocation::HLT))*tempSensorsNo);
    //temperatureSensors = (TemperatureSensor **) malloc(sizeof(new TemperatureSensor(NULL, TemperatureSensor::SensorLocation::HLT))*tempSensorsNo);

    /*
    for (int i = 0; i<tempSensorsNo; i++) {
      temperatureSensors[i] = NULL;
    }
    */

//    temperatureSensors[SensorLocation::HLT] = TemperatureSensor(new uint8_t [8] {0x28, 0xFF, 0x60, 0xA0, 0x64, 0x16, 0x3, 0x6F}, SensorLocation::HLT);
//    temperatureSensors[SensorLocation::MT] = TemperatureSensor(new uint8_t [8] {0x28, 0xFF, 0xD0, 0x50, 0x63, 0x16, 0x4, 0xA8}, SensorLocation::MT);
//    temperatureSensors[SensorLocation::BK] = TemperatureSensor(new uint8_t [8] {0x28, 0xFF, 0x7D, 0x34, 0x63, 0x16, 0x3, 0x6A}, SensorLocation::BK);
    temperatureSensors[SensorLocation::COOLER_OUT] = TemperatureSensor(new uint8_t [8] {0x28, 0xFF, 0xB7, 0xC3, 0x51, 0x17, 0x4, 0xD4}, SensorLocation::COOLER_OUT);
    temperatureSensors[SensorLocation::FERMENTOR] = TemperatureSensor(new uint8_t [8] {0x28, 0xFF, 0xDF, 0x8E, 0xC1, 0x16, 0x04, 0x6A}, SensorLocation::FERMENTOR);

    //New setup
    temperatureSensors[SensorLocation::MT] = TemperatureSensor(new uint8_t [8] {0x28, 0xFF, 0xA6, 0x53, 0x52, 0x17, 0x04, 0x60}, SensorLocation::HLT);
    temperatureSensors[SensorLocation::HLT] = TemperatureSensor(new uint8_t [8] {0x28, 0xFF, 0xAB, 0xC4, 0x51, 0x17, 0x04, 0x0F}, SensorLocation::BK);
    temperatureSensors[SensorLocation::BK] = TemperatureSensor(new uint8_t [8] {0x28, 0xFF, 0xAB, 0xC4, 0x51, 0x17, 0x04, 0x0F}, SensorLocation::BK); //Old BK

    // New Boil kettle (remaining prepared temp sensor)
    //temperatureSensors[SensorLocation::BK] = TemperatureSensor(new uint8_t [8] {0x28, 0xFF, 0x97, 0xD5, 0x51, 0x17, 0x04, 0x1D}, SensorLocation::MT); //New BK
}


void SensorManager::readSensors() {
  readTemperatureSensors();
}

void SensorManager::readTemperatureSensors() {

  for ( auto &p : temperatureSensors ) {
    p.second.readSensor();
    //logger->trace("%i - %.2f -  Ref: 0x%X", p.second.getLocation(), p.second.getValue(), &p.second);
  }

  /*
  for (int i = 0; i < tempSensorsNo; i++) {
    if (temperatureSensors[i] != NULL) {
      temperatureSensors[i]->readSensor();
    }
  }
  */
}

TemperatureSensor& SensorManager::getTemperatureSensor(SensorLocation sensorLocation) {
  return temperatureSensors[sensorLocation];
}

TempSensorMap& SensorManager::getAllTemperatureSensors() {
  return temperatureSensors;
}

uint8_t SensorManager::getTemperatureSensorNumber() {
  return temperatureSensors.size();
}
