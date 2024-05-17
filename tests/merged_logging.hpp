#pragma once
// #include <...> (HPPMERGE)
#include <memory>
#include <string>
#include <string_view>
#include <optional>
#include <array>
#include <vector>
#include <stack>
#include <queue>
#include <deque>
#include <unordered_set>
#include <map>
#include <set>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <source_location>

// #include "common.hpp" (HPPMERGE)
namespace Logging {
    // Types
    // Types :: address
    using address = std::size_t;
    // Types :: int
    using int8 = std::int8_t;
    using int16 = std::int16_t;
    using int32 = std::int32_t;
    using int64 = std::int64_t;
    // Types :: uint
    using uint8 = std::uint8_t;
    using uint16 = std::uint16_t;
    using uint32 = std::uint32_t;
    using uint64 = std::uint64_t;
    // Types :: string
    using string = std::string;
    // Types :: string_view
    using string_view = std::string_view;
    // Types :: optional
    template<typename T>
    using Opt = std::optional<T>;
    // Container
    // Container :: Sequence
    template<typename T, address N>
    using Array = std::array<T, N>;
    template<typename T>
    using List = std::vector<T>;
    template<typename T>
    using Stack = std::stack<T>;
    template<typename T>
    using Queue = std::queue<T>;
    template<typename T>
    using Deque = std::deque<T>;
    // Container :: Tree
    template<typename T>
    using Set = std::set<T>;
    template<typename Key, typename Value>
    using Map = std::map<Key, Value>;
    // Container :: Hash
    template<typename T>
    using HashSet = std::unordered_set<T>;
    template<typename Key, typename Value>
    using HashMap = std::unordered_map<Key, Value>;
    // Move Semantics
    using std::forward;
    using std::move;
    // Smart Pointers
    using std::unique_ptr;
    using std::shared_ptr;
    using std::weak_ptr;
}

// #include "common_stream.hpp" (HPPMERGE)
namespace Logging {
    // cout
    using std::cout;
    using std::endl;
    // io
    using std::istream;
    using std::ostream;
    // file io
    using std::fstream;
    using std::ifstream;
    using std::ofstream;
    // stringstream
    using std::stringstream;
}

// #include "meta.hpp" (HPPMERGE)
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

// #include "format.hpp" (HPPMERGE)
namespace Logging {
    // Format
    class Format {
    public:
        // constructor
        Format(string_view view)
            : m_View(view) {}
        // operator<<
        template<typename T>
        Format& operator<<(const T& param) {
            address index = m_View.find("{}");
            if (index < string::npos) {
                m_Stream << m_View.substr(0, index) << param;
                m_View = m_View.substr(index + 2);
            }
            return *this;
        }
        // close
        string close() {
            m_Stream << m_View;
            m_View = string_view();
            return m_Stream.str();
        }
    private:
        // member
        string_view m_View;
        stringstream m_Stream;
    };
}

// #include "logger.hpp" (HPPMERGE)
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