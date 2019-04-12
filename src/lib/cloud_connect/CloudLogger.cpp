#include "CloudLogger.h"

CloudLogger::CloudLogger(CloudConnect* cc, String app, String system, LogLevel level,
    const LogCategoryFilters &filters) : LogHandler(level, filters), cc(cc), m_app(app),
                                         m_system(system)  {
    LogManager::instance()->addHandler(this);
}

/// Send the log message to Cloud Connect.
void CloudLogger::log(String message) {
    String time = Time.format(Time.now(), TIME_FORMAT_ISO8601_FULL);
    String packet = String::format("<22>1 %s %s %s - - - %s", time.c_str(), m_system.c_str(), m_app.c_str(),
                                   message.c_str());
    //Serial.println(packet);
    
    //TODO: Create event
    long ttime = Time.now()*1000;
    const int capacity = JSON_OBJECT_SIZE(9+1);
    StaticJsonBuffer<capacity> jsonBuffer;
    JsonObject& root = jsonBuffer.createObject();
    root["type"] = "event";
    root["event"] = "logging";
    root["timestamp"] = ttime;
    std::string str (Time.format(ttime, TIME_FORMAT_ISO8601_FULL).c_str());
    root["timestamp_human"] = str;
    JsonObject& payload = root.createNestedObject("payload");
    std::string msg (packet.c_str());
    payload["message"] = msg.length();
    //payload["message2"] = msg;

    cc->emitEvent(root);
}

CloudLogger::~CloudLogger() {
    LogManager::instance()->removeHandler(this);
}

// The floowing methods are taken from Particle FW, specifically spark::StreamLogHandler.
// See https://github.com/spark/firmware/blob/develop/wiring/src/spark_wiring_logging.cpp
const char* CloudLogger::CloudLogger::extractFileName(const char *s) {
    const char *s1 = strrchr(s, '/');
    if (s1) {
        return s1 + 1;
    }
    return s;
}

const char* CloudLogger::extractFuncName(const char *s, size_t *size) {
    const char *s1 = s;
    for (; *s; ++s) {
        if (*s == ' ') {
            s1 = s + 1; // Skip return type
        } else if (*s == '(') {
            break; // Skip argument types
        }
    }
    *size = s - s1;
    return s1;
}

void CloudLogger::logMessage(const char *msg, LogLevel level, const char *category, const LogAttributes &attr) {
    String s;

    if (category) {
        s.concat("[");
        s.concat(category);
        s.concat("] ");
    }

    // Source file
    if (attr.has_file) {
        s = extractFileName(attr.file); // Strip directory path
        s.concat(s); // File name
        if (attr.has_line) {
            s.concat(":");
            s.concat(String(attr.line)); // Line number
        }
        if (attr.has_function) {
            s.concat(", ");
        } else {
            s.concat(": ");
        }
    }

    // Function name
    if (attr.has_function) {
        size_t n = 0;
        s = extractFuncName(attr.function, &n); // Strip argument and return types
        s.concat(s);
        s.concat("(): ");
    }

    // Level
    s.concat(levelName(level));
    s.concat(": ");

    // Message
    if (msg) {
        s.concat(msg);
    }

    // Additional attributes
    if (attr.has_code || attr.has_details) {
        s.concat(" [");
        // Code
        if (attr.has_code) {
            s.concat(String::format("code = %p" , (intptr_t)attr.code));
        }
        // Details
        if (attr.has_details) {
            if (attr.has_code) {
                s.concat(", ");
            }
            s.concat("details = ");
            s.concat(attr.details);
        }
        s.concat(']');
    }

    log(s);
}
