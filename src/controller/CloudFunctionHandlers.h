#pragma once
#include <ArduinoJson.h>

void handleCloudEvent_RefreshSensors(JsonObject& event); // Forward declaration
void handleCloudEvent_RefreshOutputs(JsonObject& event); // Forward declaration
void handleCloudFunction_HealthInfo(JsonObject& event); // Forward declaration
void handleCF_setOutputPower(JsonObject& event); // Forward declaration
void handleCF_setOutput(JsonObject& event); // Forward declaration
void handleCF_setOutputMode(JsonObject& event); // Forward declaration