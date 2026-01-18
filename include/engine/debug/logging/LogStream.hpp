#pragma once

#include <iostream>
#include <stdexcept>
#include "Log.hpp"
#include "LogCommands.hpp"
#include "engine/types/strings/hstring/hstring.hpp"

namespace gan::debug {

    template<uint32_t L, Severity S>
    /// LogStream, instead of behaving like a LogBucket, does not hold logs and just prints output to the console. Has a max character limit per logging instance.\n"
    class LogStream {
    private:
        /// Creates a stream of characters that refreshes
        fstring<L> stream;
        /// The User-defined "source" parameter of the log
        fstring<32> streamsource;
        /// the timestamp of the current log
        double timestamp;
        /// if a log's been added
        bool logged = false;

        friend class ConsoleLogger;
    public:
        LogStream() = default;

        LogStream& src(const char source[]) {
            streamsource = source;
            return *this;
        }

        str_view buffer() {
            return stream.wrap();
        }

        LogStream& operator<<(const char* msg) {
            stream << msg;
            return *this;
        }

        LogStream& operator<<(char c) {
            stream << c;
            return *this;
        }

        template<signed_int T>
        LogStream& operator<<(T v) {
            stream << static_cast<int>(v);
            return *this;
        }

        template<unsigned_int T>
        LogStream& operator<<(T v) {
            stream << static_cast<int>(v);
            return *this;
        }

        template<any_float T>
        LogStream& operator<<(T v) {
            stream << static_cast<double>(v);
            return *this;
        }

        template<uint32_t C>
        LogStream& operator<<(fstring<C> str) {
            stream << str.c_str();
            return *this;
        }

        LogStream& operator<<(void* ptr) {
            stream << ptr;
            return *this;
        }

        void operator<<(logcommands command) {
            if (command == logcommands::ENDLOG) {
                auto now = std::chrono::steady_clock::now();
                timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();

                logged = true;
            }
        }

        void operator<<(Log& l) {
            streamsource.clear();
            stream.clear();
            streamsource << l.source;
            stream << l.message;
            *this << logcommands::ENDLOG;
        }

    private:
        /// Number of stored logs
        uint16_t size() const { return stream.length(); }

        /// Capacity of the log bucket
        constexpr uint16_t capacity() const { return L; }

        /// Clear all logs
        void clear() { stream.clear(); }

    };

}