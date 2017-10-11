#ifndef BREWSTER_CONTROLLER_H
#define BREWSTER_CONTROLLER_H

#include "application.h"
#include "TemperatureSensors.h"



class BrewsterController {
public:
    static BrewsterController *get();
    ~BrewsterController() {};
    void controllerLoop();

    TemperatureSensors *temperatureSensors;

private:
    BrewsterController();
    static BrewsterController *instance;

    long i;
    Logger logger;
};


#endif BREWSTER_CONTROLLER_H
