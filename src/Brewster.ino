#include "util/BrewsterGlobals.h"
#include "util/BrewsterUtils.h"
#include "util/TempUtils.h"

#include "controller/BrewsterController.h"
#include "controller/Speaker.h"
#include "controller/LcdController.h"


//Globals
SYSTEM_MODE(SEMI_AUTOMATIC);
LogCategoryFilters logFilters;
SerialLogHandler logHandler(115200, LOG_LEVEL_ALL, logFilters);

//LCD
USARTSerial& nexSerial = Serial1;

//Threads
Thread *controllerThread;
Thread *controllerThreadTemperature;
Thread *controllerThreadOutput;
//BrewsterController controller;

//Touch Sensor Variables


void setup() {
	Serial.begin(115200);

	//Init PINS
	pinMode(BrewsterGlobals::get()->pinAC1, OUTPUT);
	pinMode(BrewsterGlobals::get()->pinAC2, OUTPUT);
	pinMode(BrewsterGlobals::get()->pinDC1, OUTPUT);
	pinMode(BrewsterGlobals::get()->pinDC2, OUTPUT);
	digitalWrite(BrewsterGlobals::get()->pinAC1, 0);
	digitalWrite(BrewsterGlobals::get()->pinAC2, 0);
	digitalWrite(BrewsterGlobals::get()->pinDC1, 0);
	digitalWrite(BrewsterGlobals::get()->pinDC2, 0);

	Log.trace("Starting application setup");
	BrewsterController::get();

	///////////////////////////////////////////////////
	//Speaker::playTheme();
	Speaker::playShortTone();
	///////////////////////////////////////////////////

	Particle.connect();
	//Connect to Cloud
	waitFor(Particle.connected, 30000);
	Time.zone(2);
	
	//I2C Setup
	if (!Wire.isEnabled()) {
    Wire.begin();
	}

	//Start controller thread
	//controllerThread = new Thread(NULL, controllerLoop);
	controllerThreadTemperature = new Thread(NULL, controllerLoopTemperature);
	controllerThreadOutput = new Thread(NULL, controllerLoopOutput);

	//Initialize first window
	//TODO: Implement handling and move to lcd_nextion
	if (BrewsterController::get()->getActiveProcess() == BrewProcesses::MASHING)
		delay(1);
	else if (BrewsterController::get()->getActiveProcess() == BrewProcesses::FERMENTING)
		delay(1);
	else
		delay(1);

	Speaker::playComplete();
	LcdController::get();
	Log.info("Setup done. Brewster is ready");

	BrewsterController::get()->initRecipe();
}

void loop(void) {
	LcdController::get()->processMessages();
}

os_thread_return_t controllerLoopTemperature(void* param) {
	for(;;) {
		BrewsterController::get()->controllerLoopTemperature();
	}
}

os_thread_return_t controllerLoopOutput(void* param) {
	for(;;) {
		BrewsterController::get()->controllerLoopOutput();
	}
}
