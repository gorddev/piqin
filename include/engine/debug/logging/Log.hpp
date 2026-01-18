#pragma once
#include <utility>

#include "../../types/strings/fstring/fstring.hpp"

namespace gan::debug {
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
        gan::fstring<128> message;
        gan::fstring<64> source;
        double timestamp;
        Log()
            : level(INFO), source(gan::fstring<64>()), timestamp(0) {}
        Log (Severity level, const char msg[], const char source[] = "")
            : message(msg), source(source), level(level) {}

        gan::str_view& to_fstring(gan::str_view& buffer) {
            auto severity = [&] {
                switch (level) {
                    case INFO:
                        return "‹Note›\t";
                    case WARNING:
                        return "|Warn|\t";
                    case ERROR:
                        return "!ERROR!\t";
                    case DEBUG:
                        return "~Debug~\t";
                    default:
                        return "?";
                }
            };

            buffer << severity();
            return buffer << '[' << precision<1>() << timestamp << "], " << source.wrap()
                                <<  "\n¨¨¨¨¨¨¨ " << message.wrap();
        }
        /// clears out the log's contents
        void clear() {
            message.clear();
            source.clear();
            timestamp = 0;
        }
    };
}
