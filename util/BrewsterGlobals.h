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

    uint8_t pinOneWire = D5;
    byte tempSensor1[8] = {0x28, 0xFF, 0x60, 0xA0, 0x64, 0x16, 0x3, 0x6F};
  	byte tempSensor2[8] = {0x28, 0xFF, 0xD0, 0x50, 0x63, 0x16, 0x4, 0xA8};
  	byte tempSensor3[8] = {0x28, 0xFF, 0x7D, 0x34, 0x63, 0x16, 0x3, 0x6A};

private:
    BrewsterGlobals() {};
    BrewsterGlobals(BrewsterGlobals const&);  // Don't Implement
    void operator=(BrewsterGlobals const&);   // Don't implement
    static BrewsterGlobals* instance;
};


#endif BREWSTER_GLOBALS_H
