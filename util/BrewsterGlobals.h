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
private:
    BrewsterGlobals() {};
    BrewsterGlobals(BrewsterGlobals const&);  // Don't Implement
    void operator=(BrewsterGlobals const&);   // Don't implement
    static BrewsterGlobals* instance;
};


#endif BREWSTER_GLOBALS_H
