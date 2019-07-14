#include "CloudFunctionHandlers.h"
#include "BrewsterController.h"
#include "../util/BrewsterGlobals.h"

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
  const int capacity = JSON_OBJECT_SIZE(1);
  StaticJsonBuffer<capacity> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["info"] = output.c_str();
  BrewsterController::get()->getCloudConnectInstance()->sendFunctionResult(event["signature"], root);
}

void handleCF_setOutputPower(JsonObject& event) {
    Log.info("Calling function handleCF_setOutputPower");


    bool isOn = event["payload"]["value"];
    std::string outputStr = event["payload"]["output"];
    ControllerOutput output;
    
    //Parse output
    if(outputStr.compare("AC1") == 0) {
        output = ControllerOutput::AC1;
    } else if(outputStr.compare("AC2") == 0) {
        output = ControllerOutput::AC2;
    } else if(outputStr.compare("DC1") == 0) {
        output = ControllerOutput::DC1;
    } else if(outputStr.compare("DC2") == 0) {
        output = ControllerOutput::DC2;
    }

    BrewsterController::get()->getOutput(output)->setPower(isOn);

    //Send result to the cloud
    const int capacity = JSON_OBJECT_SIZE(1);
    StaticJsonBuffer<capacity> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["success"] = true;
    BrewsterController::get()->getCloudConnectInstance()->sendFunctionResult(event["signature"], root);
}

void handleCF_setOutput(JsonObject& event) {
    Log.info("Calling function handleCF_setOutput");
    bool success = true;

    //Parse output
    ControllerOutput output;
    std::string outputStr = event["payload"]["output"];
    if(outputStr.compare("AC1") == 0) {
        output = ControllerOutput::AC1;
    } else if(outputStr.compare("AC2") == 0) {
        output = ControllerOutput::AC2;
    } else if(outputStr.compare("DC1") == 0) {
        output = ControllerOutput::DC1;
    } else if(outputStr.compare("DC2") == 0) {
        output = ControllerOutput::DC2;
    } else {
        success = false;
        Log.warn(String::format("Cannot set output value. Invalid output ID provided. ID: %s", outputStr.c_str()));
    }
    
    JsonObject& payload = event["payload"];
    if (success) {
        if(payload.containsKey("output_value")) {
            int targetOutput = event["payload"]["output_value"];
            BrewsterController::get()->getOutput(output)->setOutput(targetOutput);
        }else if(payload.containsKey("target_temperature")) {
            double target = event["payload"]["target_temperature"];
            BrewsterController::get()->getOutput(output)->changeTargetValue(target);
        }else {
            Log.warn("Cannot set output value. Parameter not found.");
            success = false;
        }
    }

    //Send result to the cloud
    const int capacity = JSON_OBJECT_SIZE(1);
    StaticJsonBuffer<capacity> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["success"] = success;
    BrewsterController::get()->getCloudConnectInstance()->sendFunctionResult(event["signature"], root);
}

void handleCF_setOutputMode(JsonObject& event) {
    Log.info("Calling function handleCF_setOutputMode");
    bool success = true;
    std::string message; 
    

    //Parse output
    ControllerOutput output;
    std::string outputStr = event["payload"]["output"];
    if(outputStr.compare("AC1") == 0) {
        output = ControllerOutput::AC1;
    } else if(outputStr.compare("AC2") == 0) {
        output = ControllerOutput::AC2;
    } else if(outputStr.compare("DC1") == 0) {
        output = ControllerOutput::DC1;
    } else if(outputStr.compare("DC2") == 0) {
        output = ControllerOutput::DC2;
    } else {
        success = false;
        message = String::format("Cannot set output value. Invalid output ID provided. ID: %s", outputStr.c_str()).c_str();
        Log.warn(message.c_str());
    }
    
    std::string modeStr = event["payload"]["mode"];

    if (success) {
        if(modeStr.compare("pid") == 0) {
            std::string targetSensor = event["payload"]["target_sensor"];
            SensorLocation sensorLocation;
            if(targetSensor.compare("BK")  == 0) {
                sensorLocation = SensorLocation::BK;
            } else if(targetSensor.compare("MT")  == 0) {
                sensorLocation = SensorLocation::MT;
            } else if(targetSensor.compare("HLT")  == 0) {
                sensorLocation = SensorLocation::HLT;
            } else if(targetSensor.compare("C-OUT")  == 0) {
                sensorLocation = SensorLocation::COOLER_OUT;
            } else if(targetSensor.compare("FERM")  == 0) {
                sensorLocation = SensorLocation::FERMENTOR;
            } else {
                success = false;
                message = String::format("Invalid sensor location. Cannot find sensor with ID: %s", targetSensor.c_str()).c_str();
                Log.warn(message.c_str());
            }

            if(success) {
                BrewsterController::get()->getOutput(output)->enablePIDMode(sensorLocation);
            }
        }else if(modeStr.compare("manual") == 0) {
            BrewsterController::get()->getOutput(output)->enableManualMode();
        }else {
            success = false;
            message = String::format("Invalid mode: %s", modeStr.c_str()).c_str();
            Log.warn(message.c_str());
        }
    }

    //Send result to the cloud
    const int capacity = JSON_OBJECT_SIZE(2);
    StaticJsonBuffer<capacity> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["success"] = success;
    if (!success) {
        root["message"] = message.c_str();
    }
    BrewsterController::get()->getCloudConnectInstance()->sendFunctionResult(event["signature"], root);
}