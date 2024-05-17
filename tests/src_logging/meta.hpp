#pragma once
#include "common.hpp"
#include "common_stream.hpp"
#include <source_location>

namespace Logging {
    // Level
	enum class Level {
		Trace = 1, Debug, Info, Warn, Error, Fatal
	};
    // Location
    struct Location {
        // constructor
        Location(const std::source_location& location);
        // operator<<
        friend ostream& operator<<(ostream& stream, const Location& location);
        // member
        string file;
        uint line;
        uint column;
    };
    // Timestamp
    struct Timestamp {
        // constructor
        Timestamp();
        // operator<<
        friend ostream& operator<<(ostream& stream, const Timestamp& timestamp);
        // member
        uint hours, minutes, seconds;
    };
    // MsgMeta
    struct MsgMeta {
        // constructor
        MsgMeta(Level level, const Location& location);
        // operator<<
        friend ostream& operator<<(ostream& stream, const MsgMeta& meta);
        // member
        Level level;
        Location location;
        Timestamp timestamp;
    };
    #define LOCATION ::Logging::Location(std::source_location::current())
}