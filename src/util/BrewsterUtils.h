#ifndef BREWSTER_UTILS_H
#define BREWSTER_UTILS_H

#include "application.h"
#include "BrewsterGlobals.h"
#include <map>
#include <utility>

class BrewsterUtils {
public:
    static unsigned long getSeconds(unsigned long time, TimeUOM timeUOM);
    static unsigned long convertSeconds(unsigned long timeSeconds, TimeUOM timeUOM);
    static void i2c_scanner();
    static int setPIDParameters(String params);
    static int enterDfuMode(String params);

    static std::map<String, String>* getParams(String input);
    static std::pair<String, String>* getSingleParam(String input);
};


#endif // BREWSTER_UTILS_H
