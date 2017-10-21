#include "lib/lcd/Adafruit_ILI9341.h"
#include "lib/lcd/Adafruit_GFX.h"
#include "lib/sensors/PietteTech_DHT.h"
#include "lib/touch/SPIArbiter.h"
#include "lib/touch/BrewPiTouch.h"
#include "lib/ui/AWindow.h"

#include "util/BrewsterGlobals.h"

#include "ui/WindowManager.h"
#include "ui/MainWindow.h"

#include "controller/BrewsterController.h"



#define TFT_GRID

//Globals
SYSTEM_MODE(SEMI_AUTOMATIC);
LogCategoryFilters logFilters;
SerialLogHandler logHandler(9600, LOG_LEVEL_ALL, logFilters);

//LCD
Adafruit_ILI9341 tft = Adafruit_ILI9341(A2, A0, A1);

//Window Management
AWindow *currentWindow;
WindowManager *windowManager;

//Threads
Thread *controllerThread;
//BrewsterController controller;

//Touch Sensor Variables
BrewPiTouch ts(GlobalSPIArbiter, D3, D4);
uint16_t ts_x, ts_y;
bool touchPressed = false;


void setup() {
	//LCD Setup
	Serial.begin(9600);
	Log.trace("Starting application setup");
	tft.begin();
	showLoadingScreen();

	//Init Touch sensor
	ts.init();
	//ts.calibrate(tempScreen.getTft());

	//Connect to Cloud
	Particle.connect();
	waitFor(Particle.connected, 30000);
	Time.zone(2);

	//Start controller thread
	//controllerThread = new Thread(NULL, controllerLoop);

	//Initialize first window
	windowManager = new WindowManager(&tft);
	windowManager->openWindow(WindowManager::Windows::MAIN_WINDOW);

	Log.info("Setup done. Brewster is ready");
}

void readTouch() {
	ts.update(10);
	ts_x = ts.getX();
	ts_y = ts.getY();
	//Log.trace("Touch: x=%d, y=%d", ts_x, ts_y);
}

void loop(void) {

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

	BrewsterController::get()->controllerLoop();
	windowManager->process();
}

/*
os_thread_return_t controllerLoop(void* param) {
	for(;;) {
		BrewsterController::get()->controllerLoop();
	}
}
*/
void showLoadingScreen() {
  tft.setRotation(3);
	tft.fillScreen(ILI9341_WHITE);
	tft.setCursor(50, 110);
	tft.setTextColor(ILI9341_BLACK, ILI9341_WHITE);
	tft.setTextSize(2);
	//tft.setFont(windowManager->font_standard);
	tft.println("Zaganjam aplikacijo");
}
