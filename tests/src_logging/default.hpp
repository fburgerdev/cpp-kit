#pragma once
#include "logger.hpp"

namespace Logging {
    // setDefaultLogger
    void setDefaultLogger(unique_ptr<Logger>&& logger);
    // getDefaultLogger
    Logger& getDefaultLogger();
    // Macro
    #define DEFAULT_LOGGER ::Logging::getDefaultLogger()
}