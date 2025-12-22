#pragma once
#include <cstdint>
#include <utility>

#include "debug/Debugger.hpp"
#include "debug/debug-utilities/LogHistory.hpp"

namespace geng {

    /** The EngineContext's primary role is to give all members of the engine access to various utilities, such as a shadow database, and randomization**/
    class EngineContext {
    private:
        /* ***************** */
        // Timing information //
        /* ***************** */
        /// The time of the previous render
        double prevTime = 0.0f;
        /// The current time
        double time = 0.0f;
        /// The current frame of the game
        std::uint64_t frame = 0;
        /// The current variation in time
        double dt = 0;

        /* ***************** */
        // Window information //
        /* ***************** */
        /// The width of the scene.
        uint16_t width = 200;
        /// The height of the scene.
        uint16_t height = 140;
        /// The size of one horizontal border on the screen
        uint16_t borderX = 0;
        /// The size of one vertical border on the screen
        uint16_t borderY = 0;
        /// The scale of the scene.
        float scale = 1.f;

        /// Whether we are pixel perfect or not
        bool pixel_perfect = false;
        /// Whether they request a layer change
        std::string layer_change;
    public:
        /// Lets us have debug utilities
        debug::Debugger debugger;

        EngineContext() = default;

        /// Updates the engineContext to get new timings and everything!!!
        void update(const double game_time) {
            time = game_time;
            dt = time - prevTime;
            prevTime = time;
            frame++;
        }

        /// Sets the window size of the current context. Not to be used except by the Engine.
        void _set_window_size(const uint16_t w, const uint16_t h) {
            width = w;
            height = h;
        }
        /// Sets the border size of the current context. Not to be used except by the engine
        void _set_border_size(uint16_t bx, uint16_t by) {
            borderX = bx;
            borderY = by;
        }

        /// Sets the scale of the current scene
        void set_scale(const float s) {
            scale = s;
        }

        /// Getters
        [[nodiscard]] double get_time() const { return time;}
        [[nodiscard]] uint64_t get_frame() const { return frame; }
        [[nodiscard]] double get_dt() const { return dt; }
        [[nodiscard]] uint16_t get_width() const { return width; }
        [[nodiscard]] uint16_t get_height() const { return height; }
        [[nodiscard]] uint16_t get_borderX() const { return borderX; }
        [[nodiscard]] uint16_t get_borderY() const { return borderY; }
        [[nodiscard]] float get_scale() const { return scale; }
        [[nodiscard]] bool is_pixel_perfect() const { return pixel_perfect; }

        /// Pixel perfect settings
        void set_pixel_perfect(const bool pix) {
            pixel_perfect = pix;
        }

        /* ***************** */
        // Logging & Debug //
        /* ***************** */


        void log(debug::Log& l) {
            l.source = "ø/" + l.source;
            debugger.log(l);
        }
        void log(int severity, const std::string msg, std::string src = "..") {
            debug::Log l(static_cast<debug::Severity>(severity), msg, std::move(src));
            log(l);
        }

        void enable_debug() { debugger.debug_mode = true; }
        void disable_debug() { debugger.debug_mode = false;}
        bool is_debug() { return debugger.is_debug(); }

        void set_layer_change(std::string new_layer) {
            layer_change = new_layer;
        }
        [[nodiscard]] std::string get_layer_change() {
            return layer_change;
        }

    };
}
