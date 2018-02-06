#include "lib/lcd/Adafruit_ILI9341.h"
#include "lib/lcd/Adafruit_GFX.h"
#include "lib/sensors/PietteTech_DHT.h"
#include "lib/touch/SPIArbiter.h"
#include "lib/touch/BrewPiTouch.h"
#include "lib/ui/AWindow.h"

#include "util/BrewsterGlobals.h"
#include "util/BrewsterUtils.h"
#include "util/TempUtils.h"

#include "ui/WindowManager.h"
#include "ui/MainWindow.h"

#include "controller/BrewsterController.h"
#include "controller/Speaker.h"
#include "controller/LcdController.h"


//Globals
SYSTEM_MODE(SEMI_AUTOMATIC);
LogCategoryFilters logFilters;
SerialLogHandler logHandler(115200, LOG_LEVEL_ALL, logFilters);

//LCD
USARTSerial& nexSerial = Serial1;
Adafruit_ILI9341 tft = Adafruit_ILI9341(BrewsterGlobals::get()->tftCS, BrewsterGlobals::get()->tftDC, BrewsterGlobals::get()->tftRST);

//Window Management
AWindow *currentWindow;
WindowManager *windowManager;

//Threads
Thread *controllerThread;
Thread *controllerThreadTemperature;
Thread *controllerThreadOutput;
//BrewsterController controller;

//Touch Sensor Variables
BrewPiTouch ts(GlobalSPIArbiter, BrewsterGlobals::get()->touchCS, BrewsterGlobals::get()->touchIRQ);
uint16_t ts_x, ts_y;
bool touchPressed = false;


void setup() {
	//Init PINS
	pinMode(BrewsterGlobals::get()->pinAC1, OUTPUT);
	pinMode(BrewsterGlobals::get()->pinAC2, OUTPUT);
	pinMode(BrewsterGlobals::get()->pinDC1, OUTPUT);
	pinMode(BrewsterGlobals::get()->pinDC2, OUTPUT);
	digitalWrite(BrewsterGlobals::get()->pinAC1, 0);
	digitalWrite(BrewsterGlobals::get()->pinAC2, 0);
	digitalWrite(BrewsterGlobals::get()->pinDC1, 0);
	digitalWrite(BrewsterGlobals::get()->pinDC2, 0);

	//LCD Setup
	Serial.begin(9600);


	Log.trace("Starting application setup");
	BrewsterController::get();
	tft.begin();
	showLoadingScreen();

	///////////////////////////////////////////////////
	//Speaker::playTheme();
	Speaker::playShortTone();
	///////////////////////////////////////////////////

	Particle.connect();

	analogWrite(BrewsterGlobals::get()->pinDC1, 80,30);
	analogWrite(BrewsterGlobals::get()->pinDC2, 80,70);

	//I2C Setup
	if (!Wire.isEnabled()) {
    Wire.begin();
	}

	//Init Touch sensor
	ts.init();
	//ts.calibrate(tempScreen.getTft());

	//Connect to Cloud
	waitFor(Particle.connected, 30000);
	Time.zone(2);

	//Start controller thread
	//controllerThread = new Thread(NULL, controllerLoop);
	controllerThreadTemperature = new Thread(NULL, controllerLoopTemperature);
	controllerThreadOutput = new Thread(NULL, controllerLoopOutput);

	//Initialize first window
	windowManager = new WindowManager(&tft);
	BrewsterGlobals::get()->setWindowManager(windowManager);

	if (BrewsterController::get()->getActiveProcess() == BrewProcesses::MASHING)
		windowManager->openWindow(WindowManager::Windows::WINDOW_MASHING);
	else if (BrewsterController::get()->getActiveProcess() == BrewProcesses::FERMENTING)
		windowManager->openWindow(WindowManager::Windows::WINDOW_FERMENTING);
	else
		windowManager->openWindow(WindowManager::Windows::MAIN_WINDOW);

	Speaker::playComplete();
	LcdController::get();
	Log.info("Setup done. Brewster is ready");
}

void readTouch() {
	ts.update(10);
	ts_x = ts.getX();
	ts_y = ts.getY();
	//Log.trace("Touch: x=%d, y=%d", ts_x, ts_y);
}

void loop(void) {
/*
	//Handling touch sensor
	if (ts.isTouched()) {
		touchPressed = true;
		readTouch();

		if (ts.isStable()) {
			windowManager->screenTouched(ts_x, ts_y);
		}
	}
	if (touchPressed && !ts.isTouched()) {
		touchPressed = false;
		windowManager->screenReleased();
	}


//	BrewsterController::get()->controllerLoop();
	windowManager->process();
*/

	//BrewsterController::get()->controllerLoop();
	LcdController::get()->processMessages();
}

/*
os_thread_return_t controllerLoop(void* param) {
	for(;;) {
		BrewsterController::get()->controllerLoop();
	}
}
*/

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

void showLoadingScreen() {
  tft.setRotation(3);
	tft.fillScreen(ILI9341_WHITE);
	tft.setCursor(50, 110);
	tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
	//tft.setTextSize(2);
	tft.setFont(font_standard);
	tft.println("Zaganjam aplikacijo");
}
