#define DEBUG_SERIAL_ENABLE
#define TRACE_ENABLE 1

#include "application.h"
//#include "lib/lcd_nextion/ITEADLIB_Nextion.h"
#include "controller/LcdControllerNex.h"

#include "util/BrewsterGlobals.h"
#include "util/BrewsterUtils.h"
#include "util/TempUtils.h"

#include "controller/BrewsterController.h"
#include "controller/Speaker.h"

#include "lib/papertrail/papertrail.h"

//Globals
SYSTEM_MODE(SEMI_AUTOMATIC);
STARTUP( early_init() );
LogCategoryFilters logFilters;

#ifdef TRACE_ENABLE
  SerialLogHandler logHandler(115200, LOG_LEVEL_ALL, logFilters);
#else
  SerialLogHandler logHandler(115200, LOG_LEVEL_INFO, logFilters);
#endif
// Online logging with papertrail
// https://papertrailapp.com/events
PapertrailLogHandler *papertailHandler;

//LCD
USARTSerial& nexSerial = Serial1;
LcdControllerNex *lcd;

//Threads
Thread *controllerThread;
Thread *controllerThreadController;
Thread *controllerThreadOutput;

//other
unsigned long lastHearthBeat;

void early_init()
{
  //Init PINS
 pinMode(BrewsterGlobals::get()->pinAC1, OUTPUT);
 pinMode(BrewsterGlobals::get()->pinAC2, OUTPUT);
 pinMode(BrewsterGlobals::get()->pinDC1, OUTPUT);
 pinMode(BrewsterGlobals::get()->pinDC2, OUTPUT);
 digitalWrite(BrewsterGlobals::get()->pinAC1, 0);
 digitalWrite(BrewsterGlobals::get()->pinAC2, 0);
 digitalWrite(BrewsterGlobals::get()->pinDC1, 0);
 digitalWrite(BrewsterGlobals::get()->pinDC2, 0);
}

void setup() {
  //LcdControllerNex::get();
  lcd = new LcdControllerNex();
  Serial.begin(115200);

	//Connect to Cloud
	Particle.connect();
	waitFor(Particle.connected, 30000);
	Time.zone(2);

  //delay(10000);
  #ifdef TRACE_ENABLE
    papertailHandler = new PapertrailLogHandler("logs2.papertrailapp.com", 41549, "brewster");
  #else
    papertailHandler = new PapertrailLogHandler("logs2.papertrailapp.com", 41549, "brewster");
  #endif

	Log.info("Starting application setup");
	BrewsterController::get();

	///////////////////////////////////////////////////
	//Speaker::playTheme();
	Speaker::playShortTone();
	///////////////////////////////////////////////////

	//I2C Setup
	if (!Wire.isEnabled()) {
    Wire.begin();
	}

	//Initialize first window
	//TODO: Implement handling and move to lcd_nextion

	BrewsterController::get()->initRecipe();
  //Init LCD
	lcd->showMainPage();
	Speaker::playComplete();
	lastHearthBeat = millis();

  //Start controller threads
	controllerThreadController = new Thread(NULL, controllerLoopController);
	controllerThreadOutput = new Thread(NULL, controllerLoopOutput);

	Log.info("Setup done. Brewster is ready");
}

void loop(void) {
  //Output a hearthbeat every 30 sec
	if(millis()-lastHearthBeat > 30000) {
		Log.info("...brewster is active...");
		lastHearthBeat = millis();
	}
	//LcdControllerNex::get()->processMessages();
  lcd->processMessages();
  //BrewsterController::get()->controllerLoopOther();
  //BrewsterController::get()->controllerLoopOutput();
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
