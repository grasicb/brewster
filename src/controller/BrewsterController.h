#ifndef BREWSTER_CONTROLLER_H
#define BREWSTER_CONTROLLER_H

#include "application.h"
#include "SensorManager.h"
#include "Output.h"
#include "../process/ProcessManager.h"
#include "../model/Recipe.h"

#include "../lib/sensors/DS18.h"

class BrewsterController {
public:
    static BrewsterController *get();
    ~BrewsterController() {};
    void controllerLoop();
    void controllerLoopOther();
    void controllerLoopOutput();

    Output* getOutput(ControllerOutput outputID);
    boolean isOutputActive(ControllerOutput outputID);

    DS18* getDS18Interface();

    SensorManager* getSensorManager();
    ProcessManager* getProcessManager();

    void initRecipe();
    Recipe getRecipe();

private:
    BrewsterController();
    static BrewsterController *instance;

    long i;
    Logger logger;
    Output *outputs[OUTPUT_NUMBER];
    Recipe recipe;

    SensorManager *sensorManger;
    ProcessManager *processManager;

    //boolean outputAC = false;
    unsigned long lastStateChange = millis();

    //HW Interfaces
    DS18 *ds18Interface;
};


#endif //BREWSTER_CONTROLLER_H
