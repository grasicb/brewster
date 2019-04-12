#pragma once

#include "application.h"
#include "CloudConnect.h"


class CloudLogger : public LogHandler {
    CloudConnect* cc;
    String m_app;
    String m_system;

public:
    /// Initialize the log handler.
    /// \param cc Pointer to the Cloud Connect instance
    /// \param app The name of the application in every log entry.
    /// \param system The name of the system in every log entry. Defaults to the deviceID.
    /// \para level Default log level.
    /// \param filters Category filters.
    ///
    /// Each log entry uses RFC 5424 with the following format:
    /// "<22>1 %ISO8601_TIME% %system% %app% - - - [%category%] %log_level%: %text".
    explicit CloudLogger(CloudConnect* cc, String app, String system = System.deviceID(), LogLevel level = LOG_LEVEL_INFO, const LogCategoryFilters &filters = {});
    virtual ~CloudLogger();

private:

    const char* extractFileName(const char *s);
    const char* extractFuncName(const char *s, size_t *size);
    void log(String message);

protected:
    virtual void logMessage(const char *msg, LogLevel level, const char *category, const LogAttributes &attr) override;
};
