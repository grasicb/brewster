#ifndef BREWSTER_CONTROLLER_H
#define BREWSTER_CONTROLLER_H

#include "application.h"
#include "TemperatureSensors.h"
#include "Output.h"

#define EEPROM_ACTIVE_PROCESS 0
#define EEPROM_PROCESS_START 4

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

    TemperatureSensors *temperatureSensors;

private:
    BrewsterController();
    static BrewsterController *instance;

    long i;
    Logger logger;
    BrewProcesses processActive;
    int processStarted;
    Output *outputs[OUTPUT_NUMBER];

    //boolean outputAC = false;
    unsigned long lastStateChange = millis();
};


#endif //BREWSTER_CONTROLLER_H
