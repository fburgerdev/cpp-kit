#include "default.hpp"

namespace Logging {
    // defaultLogger
    unique_ptr<Logger> s_DefaultLogger(new Logger("Logger"));
    // setDefaultLogger
    void setDefaultLogger(unique_ptr<Logger>&& logger) {
        s_DefaultLogger = move(logger);
    }
    // getDefaultLogger
    Logger& getDefaultLogger() {
        return *s_DefaultLogger;
    }
}