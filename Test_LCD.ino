#include "lib/lcd/Adafruit_ILI9341.h"
#include "lib/lcd/Adafruit_mfGFX.h"
#include "ui/TemperatureScreen.h"
#include "lib/sensors/PietteTech_DHT.h"
#include "lib/touch/SPIArbiter.h"
#include "lib/touch/BrewPiTouch.h"
#include "lib/sensors/OneWire.h"
#include "lib/sensors/DS18.h"
#include "lib/ui/IWindow.h"

#define TFT_GRID

//Globals
TemperatureScreen tempScreen = TemperatureScreen();
SYSTEM_MODE(SEMI_AUTOMATIC);
IWindow *testWin;

//Temperature Sensors
PietteTech_DHT dhtSensor(6, DHT22);
OneWire wire = OneWire(D5);
DS18 ds18Sensor(D5);
float lastDs18Temp = 0;
long lastRead = 0;
const int refreshRate = 5000;

//Touch Sensor Variables
BrewPiTouch ts(GlobalSPIArbiter, D3, D4);
uint16_t ts_x, ts_y;
bool touchPressed = false;
long lastTouchPressed = 0;
const int touchTresshold = 65;


void setup() {
	//LCD Setup
	Serial.begin(9600);
	Serial.println("Adafruit 2.2\" SPI TFT Test! _m1");

	tempScreen.showLoadingScreen();


	//Init Touch sensor
	ts.init();
	//ts.calibrate(tempScreen.getTft());
	//attachInterrupt(D4, touchPressedHandler, RISING);
	//attachInterrupt(D4, touchReleasedHandler, FALLING);

	//Connect to Cloud
	Particle.connect();
	waitFor(Particle.connected, 30000);
	Time.zone(2);

  //Setup done
	tempScreen.initScreen();
	scanOneWireAddresses();
	scanOneWireAddresses();
	scanOneWireAddresses();
	scanOneWireAddresses();
}

void touchPressedHandler() {
	//touchPressed = true;
}

void touchReleasedHandler() {
	//touchPressed = false;
	//printTouchCoordinates();
	//Serial.println("Released: ");
}

void readTouch() {
	ts.update();
	//if (ts.isStable()) {
		ts_x = ts.getX();
		ts_y = ts.getY();
	//}else{
	//	ts_x = 0;
	//	ts_y = 0;
	//}

}

void printTouchCoordinates() {
	Serial.print("Touched: ");
	Serial.print("x = ");
	Serial.print(ts_x);
	Serial.print(", y = ");
	Serial.println(ts_y);
}

void loop(void) {

	if (ts.isTouched()) {
		touchPressed = true;
		lastTouchPressed = millis();
		readTouch();
		if (ts.isStable()) {
			//printTouchCoordinates();
		 //if (millis() > lastTouchPressed+touchTresshold)
				tempScreen.screenTouched(ts_x, ts_y);
		}
	}

	if (touchPressed && !ts.isTouched()) {
		touchPressed = false;
		Serial.println("Touch released");
		tempScreen.screenReleased();
	}

/*
	if (touchPressed && millis() > lastTouchPressed+touchTresshold) {
		touchPressed = false;
		printTouchCoordinates();
		//tempScreen.drawPixel(ts_x, ts_y);
		tempScreen.screenTouched(ts_x, ts_y);
	}
*/

/*
	if (ts.isTouched()) {
		readTouch();
	}
	*/

	String time = Time.format(Time.now(), "%H:%M:%S");
	tempScreen.updateTime(time);
	if (millis() > lastRead+refreshRate) {
		Serial.println("Refreshing temperature");
		lastRead = millis();
		float humidity;
		float temp1;
		getDhtSensorData(humidity, temp1);
		if (ds18Sensor.read())
			lastDs18Temp = ds18Sensor.celsius();

		tempScreen.displayData(humidity, temp1, lastDs18Temp);
	}

}

void getDhtSensorData(float &humidity, float &temp) {
	dhtSensor.acquire();
	while (dhtSensor.acquiring());

	int result = dhtSensor.getStatus();

  if (result != DHTLIB_OK) {
		Serial.println("Error while reading DHT sensor");
	  switch (result) {
	      case DHTLIB_ERROR_CHECKSUM:
	          Serial.println("Error\n\r\tChecksum error");
	          break;
	      case DHTLIB_ERROR_ISR_TIMEOUT:
	          Serial.println("Error\n\r\tISR time out error");
	          break;
	      case DHTLIB_ERROR_RESPONSE_TIMEOUT:
	          Serial.println("Error\n\r\tResponse time out error");
	          break;
	      case DHTLIB_ERROR_DATA_TIMEOUT:
	          Serial.println("Error\n\r\tData time out error");
	          break;
	      case DHTLIB_ERROR_ACQUIRING:
	          Serial.println("Error\n\r\tAcquiring");
	          break;
	      case DHTLIB_ERROR_DELTA:
	          Serial.println("Error\n\r\tDelta time to small");
	          break;
	      case DHTLIB_ERROR_NOTSTARTED:
	          Serial.println("Error\n\r\tNot started");
	          break;
	      default:
	          Serial.println("Unknown error");
	          break;
	  }
	}else{
		humidity = dhtSensor.getHumidity();
		temp = dhtSensor.getCelsius();
	}
}


void scanOneWireAddresses() {
	byte i;
  byte present = 0;
  byte addr[8];

  if (!wire.search(addr)) {
    Serial.println("No more addresses.");
    Serial.println();
    wire.reset_search();
    //delay(250);
    return;
  }

  // if we get here we have a valid address in addr[]
  // you can do what you like with it
  // see the Temperature example for one way to use
  // this basic code.

  // this example just identifies a few chip types
  // so first up, lets see what we have found

  // the first ROM byte indicates which chip family
  switch (addr[0]) {
    case 0x10:
      Serial.println("Chip = DS1820/DS18S20 Temp sensor");
      break;
    case 0x28:
      Serial.println("Chip = DS18B20 Temp sensor");
      break;
    case 0x22:
      Serial.println("Chip = DS1822 Temp sensor");
      break;
    case 0x26:
      Serial.println("Chip = DS2438 Smart Batt Monitor");
      break;
    default:
      Serial.println("Device type is unknown.");
      // Just dumping addresses, show them all
      //return;  // uncomment if you only want a known type
  }

  // Now print out the device address
  Serial.print("ROM = ");
  Serial.print("0x");
    Serial.print(addr[0],HEX);
  for( i = 1; i < 8; i++) {
    Serial.print(", 0x");
    Serial.print(addr[i],HEX);
  }

  // Show the CRC status
  // you should always do this on scanned addresses

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.println("CRC is not valid!");
      return;
  }

  Serial.println();

  wire.reset(); // clear bus for next use
}

void readDS18() {

}
