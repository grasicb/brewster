#ifndef BREWSTER_CONTROLLER_H
#define BREWSTER_CONTROLLER_H

#include "application.h"
#include "TemperatureSensors.h"

#define EEPROM_ACTIVE_PROCESS 0
#define EEPROM_PROCESS_START 4

enum BrewProcesses {NONE, MASHING, BOILING, CHILLING, FERMENTING};

class BrewsterController {
public:
    static BrewsterController *get();
    ~BrewsterController() {};
    void controllerLoop();
    void startProcess(BrewProcesses process);
    void stopProcess();
    BrewProcesses getActiveProcess();
    int getProcessStartTime();

    TemperatureSensors *temperatureSensors;

private:
    BrewsterController();
    static BrewsterController *instance;

    long i;
    Logger logger;
    BrewProcesses processActive;
    int processStarted;

    boolean outputAC = false;
    unsigned long lastStateChange = millis();
};


#endif BREWSTER_CONTROLLER_H
