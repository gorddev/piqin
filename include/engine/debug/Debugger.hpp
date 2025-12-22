#pragma once

#include "debug-utilities/LogHistory.hpp"

namespace geng::debug {

    /** Contains a lot of information and utilities for debugging. **/
    class Debugger {
        // We make the debug boolean easily togglable.
    public:
        /// Enables debug mode or not
        bool debug_mode = false;
    private:
        /// Contains all the logs from the system
        LogHistory logs;
        /// Whether we display hitboxes or not
        bool hitboxes = true;
        /// Whether we display selected object info or not
        bool object_info = true;
        /// Whether we display the layer/scene grid or not
        bool grid = true;
    public:

        /// Allows us to add a log
        void log(Log& l) {
            // Only store if debug mode is enabled.
            if (debug_mode) {
                // Adds a log to the log history
                logs.add_log(l);
                // If it's worthy of an error, we abort
                if (l.level == 2) {
                    logs.dump_logs();
                    abort();
                }
            }
        }

        /// Sets the log severity
        void set_log_severity(int severity) {
            logs.set_severity(severity);
        }

        /// Enables immedaite print
        void enable_immediate_print() {
            logs.enable_immediate_print();
        }

        /// Disables immediate print
        void disable_immediate_print() {
            logs.disable_immediate_print();
        }

        // ******************
        // Boilerplate code
        // ******************
        /// Returns true if debug mode is enabled
        [[nodiscard]] bool is_debug() const {return debug_mode;}
        // <><><> hitboxes <><><>
        /// Enables hitboxes
        void enable_hitboxes() { hitboxes = true; }
        /// Disables hitboxes
        void disable_hitboxes() { hitboxes = false; }
        /// Toggles hitboxes
        void toggle_hitboxes() { hitboxes = !hitboxes; }
        /// Returns true if hitboxes are enabled
        [[nodiscard]] bool is_hitboxes() const { return hitboxes && debug_mode; }
        // <><><> grid <><><>
        // enables the grid
        /// enables the grid overlay
        void enable_grid() { grid = true; }
        /// disables the grid overlay
        void disable_grid() { grid = false; }
        /// toggles the grid
        void toggle_grid() { grid = !grid; }
        /// returns true if the grid is enabled
        [[nodiscard]] bool is_grid() const { return grid && debug_mode; }


    };
}
