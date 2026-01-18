#pragma once
#include "engine/layers/LayerState.hpp"
#include "engine/scene/morphs/Morph.hpp"

namespace gfx {
    // Shake type macros
    enum class ShakeType : uint8_t {
        RANDOM = 0,
        CIRCULAR = 1,
        FLOATY = 2,
        SIDE = 3
    };

    /** @brief Shakes are a subclass of an Morph. To add a Shake, use @code layer.add_morph(...)@endcode.
     * To specify a shake's type, assign it a @code GENG_Shake@endcode enum in its constructor.
     */
    class Shake : public gan::Morph {

    public:
        /// Constructor for the shaking!
        Shake(gan::Gear* g, ShakeType shakeType, float amplitude, float speed = 1.0, float duration = -1.f, bool decay = false);

        /// The Morph override
        bool update(gan::LayerState &time) override;

    protected:
        /// Speed of the shake
        float speed;
        /// Type of shake (random pos, circular, "floaty")
        ShakeType shakeType;
        /// Whether you want the amplitude of displacement to decay
        bool decay;
        /// Determines if a shake is complete in all three direction (x,y,z).
        uint8_t complete = false;

        // Various functions for each shake type.
        void shake_random(gan::Transform2D& tref);
        void shake_circular(gan::Transform2D &tref, gan::LayerState &time) const;
        void shake_floaty(gan::Transform2D &tref, gan::LayerState &time) const;
        void shake_side(gan::Transform2D &tref, gan::LayerState &time) const;

    };
}

