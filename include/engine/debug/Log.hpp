#pragma once
#include <string>
#include <utility>

namespace geng::debug {
    /** Severity has four different kinds of levels that can be filtered through:
     * - @code INFO@endcode › General System Info
     * - @code WARNING@endcode › Users should watch out and void this behavior if possible
     * - @code ERROR@endcode › Errors should crash the program immediately
     * - @code DEBUG@endcode › User-specified channel for their own debugs
     */
    enum Severity {
        INFO = 0,
        WARNING = 1,
        ERROR = 2,
        DEBUG = 3
    };
    static bool operator>(const Severity lhs, const Severity rhs) {
        return static_cast<uint8_t>(lhs) > static_cast<uint8_t>(rhs);
    }

    /** Class that makes up one log **/
    struct Log {
        Severity level;
        std::string message;
        std::string source;
        double timestamp;
        Log (Severity level, std::string message, std::string source = "")
            : message(std::move(message)), source(std::move(source)), level(level) {}

        std::string to_string() const {
            auto severity = [&] {
                switch (level) {
                    case INFO:
                        return "‹Info›\t";
                    case WARNING:
                        return "|Warn|\t";
                    case ERROR:
                        return "!ERROR!\t";
                    case DEBUG:
                        return "~Debug~\t";
                    default:
                        return "ø";
                }
            };

            std::string ret = severity();
            ret += "[" + std::to_string(timestamp) + "], " + source
                                +   "\n¨¨¨¨¨¨¨ " + message;
            return ret;
        }
    };
}
