#pragma once
#include "common_stream.hpp"

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