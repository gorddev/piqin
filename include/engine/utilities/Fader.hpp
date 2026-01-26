#pragma once
#include <cmath>
#include <cstdint>

namespace gutils {

    /// Used for consistently fading things in and out across the engine.
    struct Fader {
        // <><><> Fading <><><>
        /// Progress of the internal fading
        float fade_progress;
        /// Speed of the internal fading
        float speed;
        /// Special class enum for fading in and out
        enum Fade : uint8_t {
            FADE_OUT = 0,
            FADE_IN = 1,
            FADE_NONE = 2,
            FADE_WAVE = 3,
            FADE_OPAQUE = 4,
            FADE_TRANSPARENT = 5,
            FADE_MAX = 6
        };
        /// Fading status
        Fade fade_status = FADE_NONE;

        /// Contains the information for waving
        struct WaveInfo {
            float min = 0.8;
            bool on;
            // Default constructor
            WaveInfo(bool wave) : on(wave) {}
            WaveInfo(bool wave, float min) : min(min), on(wave) {}
        };
        /// Waving information.
        WaveInfo wave;

        /// Constructor for a fader
        Fader(float fade_speed, WaveInfo wave, Fade starting_progress = FADE_TRANSPARENT)
            : speed(fade_speed), wave(wave)
        {
            // If the user wants the object to start faded out
            if (starting_progress == FADE_TRANSPARENT) {
                fade_progress = 0.f;
                fade_status = FADE_NONE;
            }
            // Otherwise, it starts faded in.
            else {
                fade_progress = wave.min;
                fade_status = FADE_NONE;
            }
        }

        /// Updates the fader
        void update(double dt, double time) {
            // Update the fader in accordance with the dt and current time

            // First get the progress delta
            float progress_delta = (dt * speed)*0.004f;

            // Then we update with fade status
            if (fade_status != FADE_NONE && fade_status != FADE_MAX) {
                switch (fade_status) {
                    // while fading in
                    case FADE_IN:
                        fade_progress += progress_delta;
                        // if waving is enabled
                        if (wave.on && fade_progress > wave.min + (1-wave.min)*sinf(time*0.005f)) {
                            fade_status = FADE_WAVE;
                            fade_progress = wave.min + (1-wave.min)*sinf(time*0.005f);
                        }
                        // otherwise stop at max
                        else if (fade_progress > 1.f) {
                            fade_status = FADE_MAX;
                            fade_progress = 1.f;
                        }
                        break;
                        // while fading out
                    case FADE_OUT:
                        fade_progress -= progress_delta;
                        if (fade_progress < 0) {
                            fade_status = FADE_NONE;
                            fade_progress = 0;
                        }
                        break;
                        // while waving up
                    case FADE_WAVE:
                        fade_progress = wave.min + (1-wave.min)*sinf(time*0.005f);
                        break;
                        // otherwise do nothing
                    default:
                        break;
                }
            }
        }

        /// Starts fading the fader in.
        void fade_in() {
            fade_status = FADE_IN;
        }

        /// Starts fading the fader out
        void fade_out() {
            fade_status = FADE_OUT;
        }

        /// Sets the progress of the fader
        void set_fader_progress(float progress) {
            fade_progress = progress;
        }

        /// Gets the progress of the fader
        [[nodiscard]] float get_fader_progress() const {
            return fade_progress;
        }

        /// Returns the alpha of a color given fader progress with a given base alpha
        uint_fast8_t calc_alpha(uint8_t base_alpha) {
            return static_cast<uint_fast8_t>(
                std::lround(fade_progress * base_alpha)
            );
        }
    };
}
