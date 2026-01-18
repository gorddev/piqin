#pragma once

#include <SDL_scancode.h>
#include "LayerFlag.hpp"

namespace gan {

    /** LayerState keeps track of the relative time of the layer, and keyboard input, such that it's simple to run at something like half-speed **/
    class LayerState final {
    private:
        /// Speed of everything in the layer
        float speed = 1.0f;
        /// The change in time of the layer.
        double dt = 0.f;
        /// The frame number of the layer
        uint64_t frame = 0;
        /// The time of the layer
        double time = 0;
        /// Pointer to all the held keys vs. not held keys
        uint8_t* held_keys = nullptr;
        /// Reference to the layer flag
        LayerFlag& lflag;
        /// Holds references to the true scene width and height
        const uint16_t& canvas_width;
        const uint16_t& canvas_height;
    public:
        /// Default constructor
        explicit LayerState(LayerFlag& lflag, const uint16_t& canvas_width, const uint16_t& canvas_height)
            : lflag(lflag), canvas_width(canvas_width), canvas_height(canvas_height) {};

        /// Updates the current time
        void _update(double& delta_time) {
            dt = speed * delta_time;
            time += dt;
            frame++;
        }

        /// Returns true if a key is held
        bool is_held(SDL_Scancode key) {
            if (static_cast<bool>(lflag & LayerFlag::active) && static_cast<bool>(lflag & LayerFlag::running)) {
                if (key < 512 && held_keys != nullptr)
                    return held_keys[key];
            }
            return false;
        }

        /// Sets the speed of the layer
        void set_speed(float new_speed) { speed = new_speed; }

        // Getters
        [[nodiscard]] float get_speed() const { return speed; }
        [[nodiscard]] uint64_t get_frame() const { return frame; }
        [[nodiscard]] double get_time() const { return time; }
        [[nodiscard]] double get_dt() const { return dt; }

        /// Sets the held keys of the layer
        void _set_held_keys(uint8_t* keys) {
            held_keys = keys;
        }

        /// gets canvas width and canvas height
        uint16_t get_canvas_width() const { return canvas_width; }
        uint16_t get_canvas_height() const { return canvas_height; }
    };

}
