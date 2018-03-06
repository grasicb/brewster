#ifndef BREWSTER_GLOBALS_H
#define BREWSTER_GLOBALS_H

#include "application.h"
#include "mutex"
#include "map"

//Global switches
#define BUTTON_TONE 0
#define OUTPUT_NUMBER 4

//Global structures
const static uint8_t sensorLocationSize = 8;

enum BrewProcess {NONE, MASHING, BOILING, CHILLING, FERMENTING};
enum ProcessState {STOPPED, STARTED, PAUSED};
enum TimeUOM {second, minute, hour, day};
enum QtyUOM {g, kg, ml, l};
enum SensorLocation {HLT, MT, BK, COOLER_IN, COOLER_OUT, FERMENTOR, FRIDGE, ROOM};
enum ControllerOutput {AC1, AC2, DC1, DC2};

const static String ProcessStateNames[] = {"Stopped", "Started", "Paused"};
const static String TimeUOMNames[4] = {"s", "min", "h", "day(s)"};
const static String QtyUOMNames[4] = {"g", "kg", "ml", "l"};
const static String sensorNames[8] = {"Hot liquor tank", "Mash tun", "Boil kettle", "Cooler input", "Coller output", "Fermentor", "Fridge", "Room"};
const static String sensorShortNames[8] = {"HLT", "MT", "BK", "C-IN", "C-OUT", "FERM", "FRDG", "ROOM"};
const static String OutputNames[] = {"AC 1", "AC 2", "DC 1", "DC 2"};


//EEPROM Locations
#define EEPROM_ACTIVE_PROCESS 0
#define EEPROM_PROCESS_START 4
#define EEPROM_RECIPE 1000
const static int EEPROM_PROCESS_DATA[] = {0, 8, 18, 28, 38};



//Global type definitions
const static ControllerOutput mashHeater = ControllerOutput::AC1;
const static ControllerOutput mashPump = ControllerOutput::DC1;
const static ControllerOutput boilHeater = ControllerOutput::AC1;
const static uint8_t mashPumpFlowRate = 80;
const static uint8_t boilHeatingRate = 80;
const static float boilingPoint = 24;


class BrewsterGlobals {
public:
    static BrewsterGlobals* get();
    ~BrewsterGlobals( );

    //PIN Assignment
    static const uint8_t pinOneWire = 0x18; //0x18 - this is via i2c BUS
    static const uint8_t pinAC1 = WKP;
    static const uint8_t pinAC2 = A6;
    static const uint8_t pinDC1 = D3;
    static const uint8_t pinDC2 = D2;
    static const uint8_t pinSpeaker = D4;

    /* Not used anymore
    static const uint8_t tftCS = A2;
    static const uint8_t tftDC = A0;
    static const uint8_t tftRST = A1;
    static const uint8_t touchCS = D6;
    static const uint8_t touchIRQ = D7;
    */

    static const uint8_t addrOneWire = 0x18;

    uint8_t global_spi_clock = 0;

    Mutex i2cMutex;

private:
    BrewsterGlobals();
    BrewsterGlobals(BrewsterGlobals const&);  // Don't Implement
    void operator=(BrewsterGlobals const&);   // Don't implement
    static BrewsterGlobals* instance;
    Logger *logger;
};

#endif // BREWSTER_GLOBALS_H
