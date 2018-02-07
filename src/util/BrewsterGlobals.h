  #ifndef BREWSTER_GLOBALS_H
#define BREWSTER_GLOBALS_H

#include "mutex"
#include "application.h"
#include "../lib/ui/AWindowManager.h"

//static Mutex lcdMutex;

#define BUTTON_TONE 0
#define OUTPUT_NUMBER 4

class BrewsterGlobals {
public:
    static BrewsterGlobals* get();
    ~BrewsterGlobals( );
    void setWindowManager(AWindowManager *windowManager) {this->windowManager = windowManager;};

    //PIN Assignment
    static const uint8_t pinOneWire = 0x18; //0x18 - this is via i2c BUS
    static const uint8_t pinAC1 = WKP;
    static const uint8_t pinAC2 = A6;
    static const uint8_t pinDC1 = D2;
    static const uint8_t pinDC2 = D3;
    static const uint8_t pinSpeaker = D4;
    //static const uint8_t pinDC3 = D4;
    static const uint8_t tftCS = A2;
    static const uint8_t tftDC = A0;
    static const uint8_t tftRST = A1;
    static const uint8_t touchCS = D6;
    static const uint8_t touchIRQ = D7;

    static const uint8_t addrOneWire = 0x18;

    uint8_t global_spi_clock = 0;

    Mutex i2cMutex;

private:
    BrewsterGlobals() {};
    BrewsterGlobals(BrewsterGlobals const&);  // Don't Implement
    void operator=(BrewsterGlobals const&);   // Don't implement
    static BrewsterGlobals* instance;
    AWindowManager *windowManager = NULL;
};


#endif // BREWSTER_GLOBALS_H
