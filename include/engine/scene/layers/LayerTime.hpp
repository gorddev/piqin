#pragma once
#include <cstdint>

namespace geng {

    /** LayerTime keeps track of the relative time of the layer, such that it's simple to run at something like half-speed **/
    class LayerTime {
    private:
        /// Speed of everything in the layer
        float speed = 1.0f;
        /// The change in time of the layer.
        double dt = 0.f;
        /// The frame number of the layer
        uint64_t frame = 0;
        /// The time of the layer
        double time = 0;
    public:
        /// Default constructor
        LayerTime() = default;

        /// Updates the current time
        void _update(double& delta_time) {
            dt = speed * delta_time;
            time += dt;
            frame++;
        }

        /// Sets the speed of the layer
        void set_speed(float new_speed) { speed = new_speed; }

        // Getters
        [[nodiscard]] float get_speed() const { return speed; }
        [[nodiscard]] uint64_t get_frame() const { return frame; }
        [[nodiscard]] double get_time() const { return time; }
        [[nodiscard]] double get_dt() const { return dt; }
    };

}
