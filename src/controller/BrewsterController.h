#ifndef BREWSTER_CONTROLLER_H
#define BREWSTER_CONTROLLER_H

#include "application.h"
#include "SensorManager.h"
#include "Output.h"
#include "../process/ProcessManager.h"
#include "../model/Recipe.h"

#include "../lib/sensors/DS18.h"
#include "../lib/cloud_connect/CloudConnect.h"

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

    void setCloudConnectInstance(CloudConnect* cc) {this->cc = cc;};
    CloudConnect* getCloudConnectInstance() {return cc;};

    void initRecipe();
    Recipe *getRecipe();
    void initProcesses();

private:
    BrewsterController();
    static BrewsterController *instance;

    long i;
    Logger logger;
    Output *outputs[OUTPUT_NUMBER];
    Recipe *recipe;
    CloudConnect* cc = NULL;

    SensorManager *sensorManger;
    ProcessManager *processManager;

    unsigned long lastStateChange = millis();

    //HW Interfaces
    DS18 *ds18Interface;
};


#endif //BREWSTER_CONTROLLER_H
