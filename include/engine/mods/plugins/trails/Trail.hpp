#pragma once
#include <engine/core/gears/Gear.hpp>
#include "engine/layers/layer-subobjects/LayerState.hpp"

namespace gfx {

    struct Trail : gan::Gear {
        /// Gear the Trail is attached to.
        Gear& payload;

        /// Must initialize a gear to attach the trail to
        explicit Trail(Gear& payload)
            : Gear({{0, 0}, 0, 0}), payload(payload) {}

        /// Updates the trail. Return true if the trail is to be deleted.
        virtual bool update(gan::LayerState& state) = 0;

        /// Displays the trail. Must override
        void to_vertex(gan::RenderBuffer &buffer) override = 0;
    };
}
