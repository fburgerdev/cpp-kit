#include "logger.hpp"
#include <mutex> // std::mutex, std::lock_guard

namespace Logging {
    // mutex
    static std::mutex s_StreamMutex;
    // Logger
    // Logger :: constructor
    Logger::Logger(const string& name)
        : m_Name(name), m_Filestream(), m_Level(Level::Trace) {}
    Logger::Logger(const string& name, const string& logfile)
        : m_Name(name), m_Filestream(logfile), m_Level(Level::Trace) {}
    // Logger :: stream
    void Logger::stream(const MsgMeta& meta, const string& msg) {
        // color
        string ansicolor;
        switch (meta.level) {
        case Level::Trace:
            ansicolor = "\033[37m";
            break;
        case Level::Debug:
            ansicolor = "\033[34m";
            break;
        case Level::Info:
            ansicolor = "\033[32m";
            break;
        case Level::Warn:
            ansicolor = "\033[33m";
            break;
        case Level::Error:
            ansicolor = "\033[31m";
            break;
        case Level::Fatal:
            ansicolor = "\033[31m\033[4m";
            break;
        default:
            break;
        }
        // stream
        std::lock_guard lock(s_StreamMutex);
        cout << ansicolor << '[' << m_Name << "] " << meta << "\n\033[0m\033[38;5;245m" << msg << "\033[0m\n" << endl;
        if (m_Filestream) {
            m_Filestream.value() << '[' << m_Name << "] " << meta << '\n' << msg << endl;
        }
    }
    // Logger :: level
    Level Logger::getLevel() const {
        return m_Level;
    }
    void Logger::setLevel(Level level) {
        m_Level = level;
    }
}