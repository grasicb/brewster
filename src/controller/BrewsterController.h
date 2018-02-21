#ifndef BREWSTER_CONTROLLER_H
#define BREWSTER_CONTROLLER_H

#include "application.h"
#include "SensorManager.h"
#include "Output.h"
#include "../model/Recipe.h"

#include "../lib/sensors/DS18.h"

enum BrewProcesses {NONE, MASHING, BOILING, CHILLING, FERMENTING};
const static String BrewProcessNames[] = {"None", "Mashing", "Boiling", "Chilling", "Fermenting"};

enum ControllerOutput {AC1, AC2, DC1, DC2};
const static String OutputNames[] = {"AC 1", "AC 2", "DC 1", "DC 2"};


class BrewsterController {
public:
    static BrewsterController *get();
    ~BrewsterController() {};
    void controllerLoop();
    void controllerLoopTemperature();
    void controllerLoopOutput();
    void startProcess(BrewProcesses process);
    void stopProcess();
    BrewProcesses getActiveProcess();
    int getProcessStartTime();
    Output* getOutput(ControllerOutput outputID);
    boolean isOutputActive(ControllerOutput outputID);
    DS18* getDS18Interface();
    SensorManager* getSensorManager();
    void initRecipe();
    Recipe getRecipe();

private:
    BrewsterController();
    static BrewsterController *instance;

    long i;
    Logger logger;
    BrewProcesses processActive;
    int processStarted;
    Output *outputs[OUTPUT_NUMBER];
    Recipe recipe;

    SensorManager *sensorManger;

    //boolean outputAC = false;
    unsigned long lastStateChange = millis();

    //HW Interfaces
    DS18 *ds18Interface;
};


#endif //BREWSTER_CONTROLLER_H
