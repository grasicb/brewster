#define DEBUG_SERIAL_ENABLE
#define WEB_TRACE_ENABLE 1

#include "lib/cloud_connect/CloudConnect.h"
#include "lib/cloud_connect/CloudLogger.h"

#include "application.h"
//#include "lib/lcd_nextion/ITEADLIB_Nextion.h"
//#include "controller/LcdControllerNex.h"

#include "util/BrewsterGlobals.h"
#include "util/BrewsterUtils.h"
#include "util/TempUtils.h"

#include "controller/BrewsterController.h"
#include "controller/Speaker.h"

#include <map>

//#include "lib/papertrail/papertrail.h"

//Globals
//SYSTEM_MODE(SEMI_AUTOMATIC);
//SYSTEM_MODE(MANUAL);
STARTUP( early_init() );
LogCategoryFilters logFilters;

#ifdef DEBUG_SERIAL_ENABLE
  SerialLogHandler logHandler(115200, LOG_LEVEL_ALL, logFilters);
#else
  SerialLogHandler logHandler(115200, LOG_LEVEL_WARN, logFilters);
#endif
// Online logging with papertrail
// https://papertrailapp.com/events
//PapertrailLogHandler *papertailHandler;
CloudLogger *cloudLogger;

//Brewster Server Connection
CloudConnect *cc;
byte server[] = { 192, 168, 1, 27 };
int port = 8081;
//void handleCloudEvent(JsonObject& event); // Forward declaration
void handleCloudEvent_RefreshSensors(JsonObject& event); // Forward declaration
void handleCloudEvent_RefreshOutputs(JsonObject& event); // Forward declaration
void handleCloudFunction_HealthInfo(JsonObject& event); // Forward declaration

//LCD
USARTSerial& nexSerial = Serial1;
//LcdControllerNex *lcd;

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
//  lcd = new LcdControllerNex();
  Serial.begin(115200);

	//Connect to Cloud
	Particle.connect();
	waitFor(Particle.connected, 30000);
	Time.zone(2);

  //Wait that the time is being synchronized
  waitFor(Time.isValid, 30000);

  //Connecting to Brewster Server
  cc = new CloudConnect(server, port);

  //cc->registerListener(handleCloudEvent);
  //cc->registerListener("ledDimm", handleCloudEvent_RefreshSensors);
  cc->registerListener("refreshSensors", handleCloudEvent_RefreshSensors);
  cc->registerListener("refreshOutputs", handleCloudEvent_RefreshOutputs);
  cc->registerFunction("healthInfo", handleCloudFunction_HealthInfo);

  #ifdef WEB_TRACE_ENABLE
    //papertailHandler = new PapertrailLogHandler("logs2.papertrailapp.com", 41549, "brewster", "crazy_boomer", LOG_LEVEL_INFO);
    cloudLogger = new CloudLogger(cc, "brewster", "crazy_boomer", LOG_LEVEL_INFO);
  #else
    //papertailHandler = new PapertrailLogHandler("logs2.papertrailapp.com", 41549, "brewster", "crazy_boomer", LOG_LEVEL_INFO);
    cloudLogger = new CloudLogger(cc, "brewster", "crazy_boomer", LOG_LEVEL_WARN);
  #endif

  if(!Time.isValid())
    Log.error("Time could not be set in the given reserved timeframe.");


	Log.info("Starting application setup");
	BrewsterController::get();
  BrewsterController::get()->setCloudConnectInstance(cc);

	///////////////////////////////////////////////////
	//Speaker::playTheme();
	Speaker::playShortTone();
	///////////////////////////////////////////////////

	//I2C Setup
  Log.trace("Starting 1Wire protocol");
	if (!Wire.isEnabled()) {
    Wire.begin();
	}

	//Initialize first window
	//TODO: Implement handling and move to lcd_nextion

  Log.trace("Initializing recipe");
	BrewsterController::get()->initRecipe();
	Speaker::playComplete();
	lastHearthBeat = millis();

  //Trigger processing loops to get temperature updates etc. before restoring processes
  Log.trace("Process sensors & outputs");
  BrewsterController::get()->controllerLoopOther();
  BrewsterController::get()->controllerLoopOutput();

  //Start controller threads
  Log.trace("Starting processing threads");
	controllerThreadController = new Thread(NULL, controllerLoopController);
	controllerThreadOutput = new Thread(NULL, controllerLoopOutput);

  //Restoring processes
  Log.trace("Restoring processes");
  BrewsterController::get()->initProcesses();

  Log.trace("Opening main page on UI");
//	lcd->showMainPage();

	Log.info("Setup done. Brewster is ready");


  }

void loop(void) {
  //Output a hearthbeat every 10 sec
	if(millis()-lastHearthBeat > 10000) {
    uint32_t freemem = System.freeMemory();
		Log.info(String::format("...brewster is active [free mem: %d]...", freemem));

//    Particle.publish("brewsterNotification", "...brewster is active...", PRIVATE);
		lastHearthBeat = millis();

    /*
    Log.info("Size lcd: %d", sizeof(lcd));
    Log.info("Size *lcd: %d", sizeof(*lcd));
    Log.info("Size &lcd: %d", sizeof(&lcd));
    Log.info("Size *wc: %d", sizeof(*lcd->getCurrentWindowController()));
    Log.info("Size *wc: %d", sizeof(*lcd->getCurrentWindowController()));
    
    Log.info("Size *recipe: %d", sizeof(*BrewsterController::get()->getRecipe()));
    Log.info("Size *SensorManager: %d", sizeof(*BrewsterController::get()->getSensorManager()));
    Log.info("Size *ProcessManager: %d", sizeof(*BrewsterController::get()->getProcessManager()));
    Log.info("Size *Output: %d", sizeof(*BrewsterController::get()->getOutput(ControllerOutput::AC1)));
    Log.info("Size *BrewsterController: %d", sizeof(*BrewsterController::get()));

    
    Log.info("Size *Process-BOILING: %d", sizeof(*BrewsterController::get()->getProcessManager()->getProcess(BrewProcess::BOILING)));
    Log.info("Size *Process-MASHING: %d", sizeof(*BrewsterController::get()->getProcessManager()->getProcess(BrewProcess::MASHING)));
    Log.info("Size *Process-COOLING: %d", sizeof(*BrewsterController::get()->getProcessManager()->getProcess(BrewProcess::COOLING)));
    Log.info("Size *Process-FERMENTING: %d", sizeof(*BrewsterController::get()->getProcessManager()->getProcess(BrewProcess::FERMENTING)));
    */

    
    
	}
	//LcdControllerNex::get()->processMessages();
//  lcd->processMessages();
  cc->process();
  Particle.process();
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

void handleCloudEvent_RefreshSensors(JsonObject& event) {
  std::map<SensorLocation, TemperatureSensor> sensorsMap = BrewsterController::get()->getSensorManager()->getAllTemperatureSensors();
  for ( auto &p : sensorsMap ) {
    p.second.sendTempChangeEvent();
  }
}

void handleCloudEvent_RefreshOutputs(JsonObject& event) {
  for (int i = 0; i < OUTPUT_NUMBER; i++) {
    BrewsterController::get()->getOutput((ControllerOutput)i)->sendCloudEvent();
  }
}

void handleCloudFunction_HealthInfo(JsonObject& event) {
  
  String output = String::format("Brewster health info:\n\tFree memory: %d, Uptime: %d hours\n\tParticle Cloud:%s\n\tWiFi:%s,  signal: %.02f%%, quality: %.02f%%",
              System.freeMemory(), System.uptime()/60/60, Particle.connected()?"Yes":"No", WiFi.SSID(), WiFi.RSSI().getStrength(), WiFi.RSSI().getQuality());
  Log.info(output);

  //Send result to the cloud
  const int capacity = JSON_OBJECT_SIZE(12+1);
  StaticJsonBuffer<capacity> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["info"] = output.c_str();
  cc->sendFunctionResult(event["signature"], root);
}

/*
void handleCloudEvent(JsonObject& event) {
    Log.trace("handleCloudEvent: %s", event["event"]);
    
    if (event["event"] == "ledDimm") {
        int val = event["payload"]["value"];
        Log.trace(String::format("Change led brightness to %d%c", val), '%');
    //    analogWrite(led, val*255/100);
    }
}
*/