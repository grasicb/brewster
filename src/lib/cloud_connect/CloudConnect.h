#ifndef _CLOUD_CONNECT_H
#define _CLOUD_CONNECT_H

#include <ArduinoJson.h>
#include "application.h"
#include <map>
#include <queue>

#define CCLIB_VERSION "0.1x"

class CloudConnect {
    public:
        typedef void (* eventHandlerFunc)(JsonObject& event);
        typedef std::map<String, eventHandlerFunc> EventHandlers;
        
        CloudConnect(byte* server, int port);
        boolean connect();
        void process();
        //void emitEvent(String event);
        void emitEvent(JsonObject& event);
        void registerListener(String eventType, eventHandlerFunc eventHandler);
        void deregisterListener(String eventType, eventHandlerFunc eventHandler);
        //JsonObject& prepareHeaderEvent(std::string eventType, int countArrayElements, int countObjects);
        
    private:
        void distributeEvent(JsonObject& event);
        void sendHearthBeat();
        void sendWelcomeMessage();
        Logger *logger;

        byte* server;
        int port;
        TCPClient client;

        std::queue<char*> outgoingQueue;
        long lastCommunication = 0;
        const int reconnectTime = 500;
        int reconnectRetries = 0;
        unsigned long lastConnectionAttempt = 0;
        unsigned int nextConnectionAttemptIn = reconnectTime;
        int charsAvailable = 0;
        
        EventHandlers eventHandlers;
};

#endif