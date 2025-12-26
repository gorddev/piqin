#pragma once


#include <iostream>
#include <stdexcept>
#include "engine/types/external/vector.hpp"
#include "Log.hpp"
#include "LogCommands.hpp"
#include "engine/types/strings/hstring/hstring.hpp"
namespace geng::debug {

    template<uint32_t L, Severity S>
    /// LogStream, instead of behaving like a LogBucket, does not hold logs and just prints output to the console. Has a max character limit per logging instance.\n"
    class LogStream {
    private:
        /// Creates a stream of characters that refreshes
        fstring<L> stream;
        /// The current source of the log
        fstring<32> streamsource;
        /// if a log's been added
        bool logged = false;
        /// console is a friend
        friend class Console;
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
            stream << str.cstr();
            return *this;
        }

        LogStream& operator<<(void* ptr) {
            stream << ptr;
            return *this;
        }

        void operator<<(logcommands command) {
            if (command == logcommands::ENDLOG) {
                auto now = std::chrono::steady_clock::now();
                double timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();

                if (streamsource.empty())
                    std::cerr << "[" << timestamp << "] ~dev > " << stream.cstr() << std::endl;
                else
                    std::cerr << "[" << timestamp << "]::" << streamsource.cstr() << " ~dev > " << stream.cstr() << std::endl;
                if (S == ERROR)
                    throw std::runtime_error("Error called via. glog::err");
                stream.clear();
                streamsource.clear();
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