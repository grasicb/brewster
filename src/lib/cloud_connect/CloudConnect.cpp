//To do:
// - implement event handled
// - implement queue in case there is no connection

#include "CloudConnect.h"

CloudConnect::CloudConnect(byte* server, int port) {
    logger = new Logger("CloudConnect");
    this->server = server;
    this->port = port;
    
    logger->trace("Initializing CloudConnect instance");
    connect();
}

bool CloudConnect::connect() {
    logger->info(String::format("Connecting to brewster communicator server(IP: %d.%d.%d.%d, Port: %d).", server[0], server[1], server[2], server[3], port));
    
    if (client.connect(server, port) && client.connected())
    {
        logger->trace("Connected to Brewster server");
        sendWelcomeMessage();
        reconnectRetries = 0;
        nextConnectionAttemptIn = reconnectTime;

        return true;
    }else{
        if (reconnectRetries>0 && nextConnectionAttemptIn < 3000) {
            nextConnectionAttemptIn *= 2;
        }
        reconnectRetries++;
        
        logger->warn(String::format("Connection to brewster server failed. Retried %d time(s), next retry in %d ms.", reconnectRetries, nextConnectionAttemptIn));

        return false;
    }
}

void CloudConnect::process() {
    if (!client.connected()) {
        if(lastConnectionAttempt + nextConnectionAttemptIn < millis()) {
            logger->warn("Client disconnected from server. Trying to reconnect.");
            connect();
            lastConnectionAttempt = millis();
        }
    } else {

        /////////////////////////////////////////////
        // Receive messages from the server if any
        charsAvailable = client.available();
        if (charsAvailable)
        {
            //logger->trace(String::format("Chars in buffer: %d", charsAvailable));
            lastCommunication = millis();           
            
            String input = "";
            while(client.available()) {
                char c = (char)client.read();
                /*
                if (c == 0)
                    logger->info("NULL character detected");
                */

                input += c;
            }
            logger->trace(String::format("Message received (%d chars): "+input, input.length()));

            const int capacity = JSON_OBJECT_SIZE(25);
            StaticJsonBuffer<capacity> jsonBuffer;
            JsonObject& root = jsonBuffer.parseObject(input.c_str());
            if (root.success()) {
                if (root["type"] == "event")
                    distributeEvent(root);
                else if (root["type"] == "function")
                    distributeFunctionCall(root);
            }else{
                logger->error("Parsing of JSON input message failed ("+input+")");
            }
        }
        
        /////////////////////////////////////////////
        //Send messages to the server from the queue
        while (!outgoingQueue.empty() && client.available()==0) {
            //ulong start = millis();
            char* event = outgoingQueue.front();
            outgoingQueue.pop();

            client.write((const u_int8_t*)event, strlen(event), 100);
            client.write((uint8_t)0, 100);

            delete event;
            //ulong t1 = millis() - start;
            //Serial.println(String::format("EVENT____ setn to the server in %u ms.", t1));
        }
    }
}
/*
void CloudConnect::emitEvent(String event) {
    if (client.connected()) {
        client.println(event);
    }else{
        logger->error("Cannot emit event to the server. Connection to the server is not established. Event: "+event);
    }
}
*/
void CloudConnect::emitEvent(JsonObject&  event) {
    // Dropping old messages after treshold is reached
    if (outgoingQueue.size() > 40) {
        char* item = outgoingQueue.front();
        outgoingQueue.pop();
        delete item;
    }


    // Adding message to the queue
    size_t size = event.measureLength()+1;
    char* str2 = (char*)malloc(size);
    event.printTo(str2, size);
    //str2[size-1] = 0;
    outgoingQueue.push(str2);
}

void CloudConnect::sendFunctionResult(const char* functionCallID, JsonObject& result) {
    ulong ttime = Time.now();
    const int capacity = JSON_OBJECT_SIZE(12+1);
    StaticJsonBuffer<capacity> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["type"] = "functionResult";
    root["signature"] = functionCallID;
    root["timestamp"] = ttime*1000;
    std::string str (Time.format(ttime, TIME_FORMAT_ISO8601_FULL).c_str());
    root["timestamp_human"] = str;
    root["result"] = result;

    emitEvent(root);
}

void CloudConnect::registerListener(String eventType, cloudHandlerFunc eventHandler) {
    this->eventHandlers[eventType] = eventHandler;
}

void CloudConnect::deregisterListener(String eventType, cloudHandlerFunc eventHandler) {
    this->eventHandlers.erase(eventType);
}

void CloudConnect::registerFunction(String function, cloudHandlerFunc eventHandler) {
    this->functionHandlers[function] = eventHandler;
}

void CloudConnect::deregisterFunction(String function, cloudHandlerFunc eventHandler) {
    this->functionHandlers.erase(function);
}
        
void CloudConnect::distributeEvent(JsonObject& event) {
    String eventType = String((const char*) event["event"]);
    cloudHandlerFunc handler = eventHandlers[eventType];

    if (handler != NULL) {
        handler(event);
    }else {
        logger->warn("No event handler defined for event: " + eventType);
    }
}

void CloudConnect::distributeFunctionCall(JsonObject& functionCall) {
    String function = String((const char*) functionCall["function"]);
    cloudHandlerFunc handler = functionHandlers[function];

    if (handler != NULL) {
        handler(functionCall);
    }else {
        logger->warn("No functional handler defined for function: " + function);
    }
}

void CloudConnect::sendHearthBeat() {
    const int capacity = JSON_OBJECT_SIZE(4);
    StaticJsonBuffer<capacity> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["type"] = "hearthbeat";
    long ttime = Time.now();
    root["timestamp"] = ttime;
    std::string str (Time.format(ttime, TIME_FORMAT_ISO8601_FULL).c_str());
    root["timestamp_human"] = str;
    
    if (client.connected()) {
        emitEvent(root);
    }else{
        logger->error("Cannot send hearthbeat to the server. Connection to the server is not established.");
    }
}

void CloudConnect::sendWelcomeMessage() {
    //const int capacity = JSON_ARRAY_SIZE(2) + 2*JSON_OBJECT_SIZE(2);
    const int capacity = JSON_OBJECT_SIZE(5);
    StaticJsonBuffer<capacity> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["type"] = "connection_established";
    long ttime = Time.now();
    root["timestamp"] = ttime;
    std::string str (Time.format(ttime, TIME_FORMAT_ISO8601_FULL).c_str());
    root["timestamp_human"] = str;
    root["client_version"] = CCLIB_VERSION;
    
    if (client.connected()) {
        emitEvent(root);
    }else{
        logger->error("Cannot send welcome message to the server. Connection to the server is not established.");
    }
}