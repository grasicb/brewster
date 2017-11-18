#ifndef BREWSTER_GLOBALS_H
#define BREWSTER_GLOBALS_H

#include "mutex"
#include "application.h"

static Mutex lcdMutex;
static String test = "test";


class BrewsterGlobals {
public:
    static BrewsterGlobals* get();
    ~BrewsterGlobals( );

    //PIN Assignment
    static const uint8_t pinOneWire = D4;
    static const uint8_t pinAC1 = WKP;
    static const uint8_t pinAC2 = A6;
    static const uint8_t pinDC1 = D2;
    static const uint8_t pinDC2 = D3;
    //static const uint8_t pinDC3 = D4;

    uint8_t global_spi_clock = 0;

    //Sensors config
    uint8_t tempSensors[10][8] = { {0x28, 0xFF, 0x60, 0xA0, 0x64, 0x16, 0x3, 0x6F},
                                        {0x28, 0xFF, 0xD0, 0x50, 0x63, 0x16, 0x4, 0xA8},
                                        {0x28, 0xFF, 0x7D, 0x34, 0x63, 0x16, 0x3, 0x6A}};
    static const uint8_t tempSensorsNo = 3;

private:
    BrewsterGlobals() {};
    BrewsterGlobals(BrewsterGlobals const&);  // Don't Implement
    void operator=(BrewsterGlobals const&);   // Don't implement
    static BrewsterGlobals* instance;
};


#endif BREWSTER_GLOBALS_H
