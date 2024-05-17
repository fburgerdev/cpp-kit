#include "meta.hpp"
#include <ctime> // timestamp

namespace Logging {
    // Location
    Location::Location(const std::source_location& location)
        : file(location.file_name()), line(location.line()), column(location.column()) {
        address index = file.find_last_of("/");
        if (index < string::npos) {
            file = file.substr(index + 1);
        }
    }
    // Location :: operator<<
    ostream& operator<<(ostream& stream, const Location& location) {
        return stream << location.file << ':' << location.line << ':' << location.column;
    }
    // Timestamp
    Timestamp::Timestamp() {
        std::time_t curr_time = std::time(NULL);
        std::tm *tm_local = std::localtime(&curr_time);
        hours = tm_local->tm_hour;
        minutes = tm_local->tm_min;
        seconds = tm_local->tm_sec;
    }
    // Timestamp :: operator<<
    ostream& operator<<(ostream& stream, const Timestamp& timestamp) {
        // hours
        if (timestamp.hours < 10) {
            stream << '0';
        }
        stream << timestamp.hours;
        stream << ':';
        // minutes
        if (timestamp.minutes < 10) {
            stream << '0';
        }
        stream << timestamp.minutes;
        stream << ':';
        // minutes
        if (timestamp.seconds < 10) {
            stream << '0';
        }
        stream << timestamp.seconds;
        return stream;
    }
    // MsgMeta
    MsgMeta::MsgMeta(Level level, const Location& location)
        : level(level), location(location), timestamp() {}
    // MsgMeta :: operator<<
    ostream& operator<<(ostream& stream, const MsgMeta& meta) {
        return stream << "Time " << meta.timestamp << " Location " << meta.location;
    }
}