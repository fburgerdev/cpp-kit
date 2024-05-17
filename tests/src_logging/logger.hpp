#pragma once
#include "meta.hpp"
#include "format.hpp"

namespace Logging {
    // Logger
    class Logger {
    public:
        // constructor
        Logger(const string& name);
        Logger(const string& name, const string& logfile);
        // log
        template<typename... TArgs>
        void log(const MsgMeta& meta, string_view fmt, const TArgs&... args) {
            if (int(m_Level) <= int(meta.level)) {
                stream(meta, (Format(fmt) << ... << args).close());
            }
        }
        // level
        Level getLevel() const;
        void setLevel(Level level);
    private:
        // stream
        void stream(const MsgMeta& meta, const string& msg);
        // member
        string m_Name;
        Opt<ofstream> m_Filestream;
        Level m_Level;
    };
    // Macros !Make sure to define DEFAULT_LOGGER before usage
    #define LOG_TRACE(...) DEFAULT_LOGGER.log(::Logging::MsgMeta(::Logging::Level::Trace, LOCATION), __VA_ARGS__)
    #define LOG_DEBUG(...) DEFAULT_LOGGER.log(::Logging::MsgMeta(::Logging::Level::Debug, LOCATION), __VA_ARGS__)
    #define LOG_INFO(...) DEFAULT_LOGGER.log(::Logging::MsgMeta(::Logging::Level::Info, LOCATION), __VA_ARGS__)
    #define LOG_WARN(...) DEFAULT_LOGGER.log(::Logging::MsgMeta(::Logging::Level::Warn, LOCATION), __VA_ARGS__)
    #define LOG_ERROR(...) DEFAULT_LOGGER.log(::Logging::MsgMeta(::Logging::Level::Error, LOCATION), __VA_ARGS__)
    #define LOG_FATAL(...) DEFAULT_LOGGER.log(::Logging::MsgMeta(::Logging::Level::Fatal, LOCATION), __VA_ARGS__)
}