#pragma once


namespace geng::debug {

    /** Contains a lot of information and utilities for debugging. **/
    class Debugger {
        // We make the debug boolean easily togglable.
    public:
        /// Enables debug mode or not
        bool debug_mode = false;
    private:
        /// Whether we display hitboxes or not
        bool hitboxes = true;
        /// Whether we display selected object info or not
        bool object_info = true;
        /// Whether we display the layer/scene grid or not
        bool grid = true;
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

    };
}
