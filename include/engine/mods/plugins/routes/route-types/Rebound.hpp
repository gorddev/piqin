#pragma once

#include "engine/layers/layer-subobjects/LayerState.hpp"
#include "engine/mods/plugins/routes/Route.hpp"

namespace gfx {
    /**
     * @brief ReboundPath moves a Transform2D away from the target, then slingshots to the target.
     */
    class Rebound final : public gfx::Route {
    public:
        /// Regular constructor
        Rebound(gan::Gear& gear, const gan::vec2 &target, float speed);
        /// Update override (true if done)
        bool update(gan::LayerState& time) override;
    };
}