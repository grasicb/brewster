#include "application.h"
#include "lib/lcd_nextion/ITEADLIB_Nextion.h"
#include "controller/LcdController.h"

#include "util/BrewsterGlobals.h"
#include "util/BrewsterUtils.h"
#include "util/TempUtils.h"

#include "controller/BrewsterController.h"
#include "controller/Speaker.h"


//Globals
SYSTEM_MODE(SEMI_AUTOMATIC);
LogCategoryFilters logFilters;
SerialLogHandler logHandler(115200, LOG_LEVEL_ALL, logFilters);

//LCD
USARTSerial& nexSerial = Serial1;

//Threads
Thread *controllerThread;
Thread *controllerThreadController;
Thread *controllerThreadOutput;

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

	//Init LCD
	LcdController::get();

	//Connect to Cloud
	Particle.connect();
	waitFor(Particle.connected, 30000);
	Time.zone(2);

	Log.trace("Starting application setup");
	BrewsterController::get();

	///////////////////////////////////////////////////
	//Speaker::playTheme();
	Speaker::playShortTone();
	///////////////////////////////////////////////////

	//I2C Setup
	if (!Wire.isEnabled()) {
    Wire.begin();
	}

	//Start controller thread
	controllerThreadController = new Thread(NULL, controllerLoopController);
	controllerThreadOutput = new Thread(NULL, controllerLoopOutput);

	//Initialize first window
	//TODO: Implement handling and move to lcd_nextion

	BrewsterController::get()->initRecipe();
	LcdController::get()->showMainPage();
	Speaker::playComplete();
	Log.info("Setup done. Brewster is ready");
}

void loop(void) {
	LcdController::get()->processMessages();
}

os_thread_return_t controllerLoopController(void* param) {
	for(;;) {
		BrewsterController::get()->controllerLoopOther();
	}
}

os_thread_return_t controllerLoopOutput(void* param) {
	for(;;) {
		BrewsterController::get()->controllerLoopOutput();
	}
}
