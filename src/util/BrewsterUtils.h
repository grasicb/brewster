#ifndef BREWSTER_UTILS_H
#define BREWSTER_UTILS_H

#include "application.h"
#include "BrewsterGlobals.h"

class BrewsterUtils {
public:
    static unsigned long getSeconds(unsigned long time, TimeUOM timeUOM);
    static unsigned long convertSeconds(unsigned long timeSeconds, TimeUOM timeUOM);
    static void i2c_scanner();
};


#endif // BREWSTER_UTILS_H
