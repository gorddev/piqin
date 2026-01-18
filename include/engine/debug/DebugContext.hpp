#pragma once


namespace gan::debug {

    /** Contains a lot of information and utilities for debugging. **/
    class DebugContext {
        // We make the debug boolean easily togglable.
    public:
        /// Enables debug mode or not
        bool debug_mode = false;
    private:
        /// Whether we display hitboxes or not
        bool hitboxes = true;
        /// Whether we display the layer/scene grid or not
        bool grid = true;
        /// Recovery mode
        bool recovery = false;
        /// Whether we're displaying in verbose mode or not
        bool verbose = false;
        /// Whether we want to move to the next layer or not
        bool change_layer = false;
        /// Pause current layer or not
        bool pause_layer = false;
    public:

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
        // <><><><> Recovery <><><><>
        /// returns true if in recovery mode
        [[nodiscard]] bool is_recovery() { return recovery; }
        // <><><> debug mode <><><>
        /// toggles debug mode
        void toggle_debug() { debug_mode = !debug_mode; }
        // <><><> verbose <><><>
        bool is_verbose() { return verbose; }
    private:
        // Only the engine can enable/disable recovery mode
        friend class Engine;
        friend class Console;
        friend class DebugButtonPanel;
        // <><><> recovery <><><>
        // enables recovery mode
        /// enables recovery mode
        void enable_recovery() { recovery = true; }
        /// disables recovery mode
        void disable_recovery() { recovery = false; }
    };
}
