#pragma once

#include "engine/layers/layer-subobjects/LayerState.hpp"
#include "engine/mods/plugins/morphs/Morph.hpp"

namespace gfx {

    /** Stretch is a permanent morph that stretches an object when it moves. **/
    class Stretch final : public gan::Morph{
    private:
        /// @brief Stores the information necessary to stretch an objects
        /// @details Below are each of the member variabels
        /// - @code float snappiness@endcode - how quickly the object moves to it's snapped position.
        /// - @code float rebound@endcode - how quickly the object returns to its original shape
        /// - @code float tolerance@endcode - how much the object must move to activate the stretch
        /// - @code float amplitude@endcode - how much the objects stretches
        struct StretchInfo {
            /// How quickly the object moves to its snapped shape.
            float snappiness;
            /// How fast the object returns to its original shape
            float rebound;
            /// How much the object must move to activate the stretch
            float tolerance;
            /// How small the object gets
            float amplitude;

            StretchInfo(float snappiness, float tolerance, float amplitude)
                : snappiness(snappiness), rebound(snappiness),tolerance(tolerance), amplitude(amplitude) {}
            StretchInfo(float snappiness, float tolerance, float amplitude, float rebound)
                : snappiness(snappiness), rebound(rebound), tolerance(tolerance), amplitude(amplitude) {}
        };
        /// Info object containing the stretch
        StretchInfo s;

        /// Previous position of the attached object
        gan::vec2 oldPos;
        /// Enum to store the current stretching state
        enum Mode : uint8_t {
            STRETCH_IN = 0,
            STRETCH_OUT = 1,
            STRETCH_NONE = 2
        };
        /// Stores whether we stretch in or out.
        short stretch_dir;
        /// Stores the current stretch state for x
        Mode state_x = STRETCH_NONE;
        /// Stores the current stretch state for y
        Mode state_y = STRETCH_NONE;

    public:
        /// Snappiness specifies how fast the object stretches.
        Stretch(gan::Gear* gear, StretchInfo stretch_info);

        /// Override function.
        bool update(gan::LayerState &time) override;
    };
}
