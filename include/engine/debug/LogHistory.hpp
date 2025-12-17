#pragma once
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include "engine/debug/Log.hpp"

namespace geng::debug {

    /**
     * @brief LogHistory manages a collection of log messages with severity tracking. Prints all logs to logs.txt.
     * @details LogHistory stores logs as individual Log objects, each of which has a severity level (0-3)
     * and a message. It allows you to:
     * 1. Add logs with a specific severity
     * 2. Filter logs based on a runtime severity threshold
     * 3. Access the top-most log by severity
     * 4. Expose logs that meet the severity threshold
     * 5. Dump logs to std::cerr in a severity-ordered format
     *
     * LogHistory maintains an internal severity filter, which determines which logs are considered "active"
     * for functions like @code top() @endcode and @code expose_logs() @endcode. This allows the user or engine
     * to dynamically adjust verbosity at runtime without rebuilding the engine.
     *
     * @note Severity levels are defined as follows:
     * - 0: Lowest / informational
     * - 1: Minor warning
     * - 2: Warning / important
     * - 3: Critical / error
     *
     * @warning The @code top() @endcode function returns a pointer to the internal Log object. Do not store
     * this pointer beyond the lifetime of the LogHistory or after clearing logs.
     */
    class LogHistory {
    private:
        std::vector<Log> logs;
        int severity_filter = 0;
        bool print_immediate = false;


    public:
        LogHistory() = default;

        void set_severity(int sev) {
            if (sev < 0) sev = 0;
            if (sev > 3) sev = 3;
            severity_filter = sev;
        }

        int get_severity() const { return severity_filter; }

        void add_log(Log log) {
            if (log.level < 0) log.level = static_cast<Severity>(0);
            if (log.level > 3) log.level = static_cast<Severity>(3);
            logs.emplace_back(log);
            if (print_immediate)
                std::cerr << log.to_string() << std::endl;
        }

        const Log* top() const {
            const Log* top_log = nullptr;
            for (const auto& log : logs) {
                if (log.level >= severity_filter) {
                    if (!top_log || log.level > top_log->level) {
                        top_log = &log;
                    }
                }
            }
            return top_log;
        }

        std::vector<Log> expose_logs() const {
            std::vector<Log> filtered;
            for (const auto& log : logs) {
                if (log.level >= severity_filter) {
                    filtered.push_back(log);
                }
            }
            return filtered;
        }

        void dump_logs() const {
            std::cerr << "************\nDumping Logs\n************\n";
            std::vector<Log> filtered = expose_logs();
            std::sort(filtered.begin(), filtered.end(),
                      [](const Log& a, const Log& b) {
                          if (a.level == b.level)
                              return a.timestamp < b.timestamp;
                          return a.level > b.level;
                      });

            for (const auto& log : filtered) {
                std::cerr << log.to_string() << std::endl;
            }
        }

        void clear() { logs.clear(); }

        void enable_immediate_print() { print_immediate = true; }
        void disable_immediate_print() { print_immediate = false; }
    };
}