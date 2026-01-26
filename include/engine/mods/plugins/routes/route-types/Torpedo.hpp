#pragma once

#include "engine/layers/layer-subobjects/LayerState.hpp"
#include "engine/mods/plugins/routes/Route.hpp"

namespace gfx {
    /**
     * @brief TorpedoPath starts an object moving slowly, and then speeds it up as it moves along.
     */
    class Torpedo final : public gfx::Route {
    private:
        /// Direction of travel for the path.
        gan::vec2 direction;
    public:
        /// Regular constructor
        Torpedo(gan::Gear& gear, const gan::vec2 &target, float speed);
        /// Update override (true if done)
        bool update(gan::LayerState& time) override;
    };
}
