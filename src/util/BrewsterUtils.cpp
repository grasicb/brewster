#include "BrewsterUtils.h"

unsigned long BrewsterUtils::getSeconds(unsigned long time, TimeUOM timeUOM) {
  switch (timeUOM) {
    case TimeUOM::second :
      return ((unsigned long)time);

    case TimeUOM::minute :
      return ((unsigned long)time)*60;

    case TimeUOM::hour :
      return ((unsigned long)time)*3600;

    case TimeUOM::day :
      return ((unsigned long)time)*86400;

    default:
      Log.error("BrewsterUtils::getSeconds: Missing conversion for TimeUOM enum %i", (int)timeUOM);
      return 0;
  }
}

unsigned long BrewsterUtils::convertSeconds(unsigned long timeSeconds, TimeUOM timeUOM) {
  switch (timeUOM) {
    case TimeUOM::second :
      return ((unsigned long)timeSeconds);

    case TimeUOM::minute :
      return ((unsigned long)timeSeconds)/60;

    case TimeUOM::hour :
      return ((unsigned long)timeSeconds)/3600;

    case TimeUOM::day :
      return ((unsigned long)timeSeconds)/86400;

    default:
      Log.error("BrewsterUtils::convertSeconds: Missing conversion for TimeUOM enum %i", (int)timeUOM);
      return 0;
  }
}

void BrewsterUtils::i2c_scanner() {
  BrewsterGlobals::get()->i2cMutex.lock();
  byte error, address;
  int nDevices;

  Log.info("Scanning I2C decives");

  nDevices = 0;
  for(address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4)
    {
      Serial.print("Unknown error at address 0x");
      if (address<16)
        Serial.print("0");
      Serial.println(address,HEX);
    }
  }
  if (nDevices == 0)
    Log.info("No I2C devices found");
  else
    Log.info("I2C scan complete");

  BrewsterGlobals::get()->i2cMutex.unlock();
}


int BrewsterUtils::setPIDParameters(String params) {
  Log.info("setPIDParameters: Setting PID parameters [%s]", (const char*) params);

  std::map<String, String>* p = getParams(params);
  std::map<String, String>::iterator it;

  BrewProcess bp = BrewProcess::NONE;
  double kp = -1;
  double ki = -1;
  double kd = -1;

  it = p->find("process");
  if(it != p->end()) {
    String value = it->second;
    Log.trace("Process: %s", (const char*) value);
    if(value.compareTo("mashing")==0)
      bp = BrewProcess::MASHING;
    else if(value.compareTo("boiling")==0)
      bp = BrewProcess::BOILING;
    else if(value.compareTo("cooling")==0)
      bp = BrewProcess::COOLING;
    else if(value.compareTo("fermenting")==0)
      bp = BrewProcess::FERMENTING;
    else if(value.compareTo("water_prep")==0)
      bp = BrewProcess::WATER_PREP;
  }

  it = p->find("p");
  if(it != p->end())
    kp = it->second.toFloat();

  it = p->find("i");
  if(it != p->end())
    ki = it->second.toFloat();

  it = p->find("d");
  if(it != p->end())
    kd = it->second.toFloat();

  if(bp != BrewProcess::NONE && kp != -1 && ki != -1 && kd != -1) {
    Log.info("setPIDParameters: Setting PID parameters for process %s [p=%.1f,i=%.1f,d=%.1f].", (const char*) (*p)["process"], kp, ki, kd);

    PidSettings ps = BrewsterGlobals::get()->getPIDSettings(bp);
    ps.kp = kp;
    ps.ki = ki;
    ps.kd = kd;
    BrewsterGlobals::get()->storePIDSettings();

    return 1;

  }else {
    String err = "";
    if(bp == BrewProcess::NONE)
      err = "process unknown";
    if(kp == -1) {
      if (err.length()>0)
        err.concat("; ");
      err.concat("invalid valid for parameter 'P'");
    }
    if(ki == -1) {
      if (err.length()>0)
        err.concat("; ");
      err.concat("invalid valid for parameter 'I'");
    }
    if(kd == -1) {
      if (err.length()>0)
        err.concat("; ");
      err.concat("invalid valid for parameter 'D'");
    }

    Log.info("setPIDParameters: Error setting PID configuration. Invalid input '%s': %s ", (const char*) params, (const char*) err);

    Log.info("Param size: %i, %s, %s, %s, %s", p->size(), (const char*) (*p)["process"], (const char*) (*p)["p"], (const char*) (*p)["i"], (const char*) (*p)["d"]);

    return 0;
  }
}

std::map<String, String>* BrewsterUtils::getParams(String input) {
  int l = input.length();
  int lastI = 0;
  int i = 0;

  std::map<String, String>* ret = new std::map<String, String>();

  while ( (i = input.indexOf(';', lastI)) >= 0) {
    std::pair<String, String>* pair = getSingleParam(input.substring(lastI, i));
    if (pair != NULL)
      ret->insert(*pair);

    lastI = i+1;
  }
  std::pair<String, String>* pair = getSingleParam(input.substring(lastI, l));
  if (pair != NULL)
    ret->insert(*pair);

  return ret;
}

std::pair<String, String>* BrewsterUtils::getSingleParam(String input) {
  std::pair<String, String>* ret = NULL;
  int i = input.indexOf('=');
  if(i>0 && i<input.length()-1) {
    String param = input.substring(0, i).trim().toLowerCase();
    String value = input.substring(i+1, input.length()).trim();
    ret = new std::pair<String, String>(param, value);

    Log.trace("getSingleParam: Input: '%s', Param:'%s', value:'%s'", (const char*) input, (const char*) param, (const char*) value);
  }else{
    Log.warn("getSingleParam: Invalid syntax for input '%s'", (const char*) input);
  }

  return ret;
}

int BrewsterUtils::enterDfuMode(String params) {
  System.dfu();
}