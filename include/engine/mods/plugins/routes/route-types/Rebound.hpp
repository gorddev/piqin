#pragma once

#include "../../../layers/layer-subobjects/LayerState.hpp"
#include "engine/mods/routes/Route.hpp"

namespace groute {
    /**
     * @brief ReboundPath moves a Transform2D away from the target, then slingshots to the target.
     */
    class Rebound final : public gan::Route {
    public:
        /// Regular constructor
        Rebound(gan::Gear& gear, const gan::vec2 &target, float speed);
        /// Update override (true if done)
        bool update(gan::LayerState& time) override;
    };
}