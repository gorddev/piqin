#pragma once

#include <iostream>
#include <stdexcept>
#include "engine/types/external/vector.hpp"
#include "Log.hpp"
#include "LogCommands.hpp"


namespace geng::debug {

    template<typename T>
    concept signed_int =
    std::signed_integral<T>;

    template<typename T>
    concept unsigned_int =
        std::unsigned_integral<T>;

    template<typename T>
    concept any_int =
        signed_int<T> || unsigned_int<T>;

    template<typename T>
    concept any_float =
        std::floating_point<T>;

    template<typename T>
    concept c_string =
        std::is_same_v<std::remove_cvref_t<T>, const char*> ||
        std::is_same_v<std::remove_cvref_t<T>, char*>;

    /// Clock used for logging
    inline auto start_time = std::chrono::steady_clock::now();

    template<uint16_t L, Severity S>
    /// the whole thing is private except constructor and overload
    class LogBucket {
    private:
        /// stores al lthe lgos
        gch::vector<Log> logs;
        /// beginning index
        uint16_t start = 0;
        /// ending index
        uint16_t count = 0;
        /// current log
        Log curlog;
        /// if a log's been added
        bool logged = false;
        /// console is a friend
        friend class DebugManager;
    public:
        LogBucket() {
            logs.resize(L);
            curlog.level = S;
        }

        LogBucket& src(const char source[]) {
            curlog.source = source;
            return *this;
        }

        LogBucket& operator<<(const char* msg) {
            curlog.message << msg;
            return *this;
        }

        LogBucket& operator<<(char c) {
            curlog.message << c;
            return *this;
        }

        template<signed_int T>
        LogBucket& operator<<(T v) {
            curlog.message << static_cast<int>(v);
            return *this;
        }

        template<unsigned_int T>
        LogBucket& operator<<(T v) {
            curlog.message << static_cast<int>(v);
            return *this;
        }

        template<any_float T>
        LogBucket& operator<<(T v) {
            curlog.message << static_cast<double>(v);
            return *this;
        }

        template<uint32_t C>
        LogBucket& operator<<(fstring<C> str) {
            curlog.message << str.cstr();
            return *this;
        }

        LogBucket& operator<<(void* ptr) {
            curlog.message << ptr;
            return *this;
        }

        void operator<<(logcommands command) {
            if (command == logcommands::ENDLOG) {
                auto now = std::chrono::steady_clock::now();
                curlog.timestamp = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();
                push(curlog);
                fstring<200> buffer;
                auto wrap = buffer.wrap();
                std::cerr << curlog.to_fstring(wrap).cstr() << std::endl;
                if (S == ERROR)
                    throw std::runtime_error("Error called via. glog::err");
                curlog.source.clear();
                curlog.message.clear();
            }
        }

        void operator<<(Log& l) {
            curlog = l;
            *this << logcommands::ENDLOG;
        }

    private:
        /// Add a new log
        void push(const Log& log) {
            if (count < L) {
                logs[(start + count)% L] = log;
                ++count;
            } else {
                logs[start] = log;
                start = (start + 1) %L;
            }
        }

        Log& operator[](uint16_t index) {
            if (index >= count)
                throw std::out_of_range("logbucked index out of range");
            return logs[(start + index) % L];
        }

        const Log& operator[](uint16_t index) const {
            if (index >= count)
                throw std::out_of_range("logbucket index out of range");
            return logs[(start + index) % L];
        }

        /// Access the oldest log
        Log& front() {
            if (count == 0) throw std::out_of_range("logbucket is empty");
            return logs[start];
        }

        const Log& front() const {
            if (count == 0) throw std::out_of_range("logbucket is empty");
            return logs[start];
        }

        /// Access the newest log
        Log& back() {
            if (count == 0) throw std::out_of_range("logbucket is empty");
            return logs[(start + count - 1) % L];
        }

        const Log& back() const {
            if (count == 0) throw std::out_of_range("logbucket is empty");
            return logs[(start + count - 1) % L];
        }

        /// Number of stored logs
        uint16_t size() const { return count; }

        /// Capacity of the log bucket
        constexpr uint16_t capacity() const { return L; }

        /// Clear all logs
        void clear() { start = 0; count = 0; }

    };

}
