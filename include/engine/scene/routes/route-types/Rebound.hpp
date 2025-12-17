#pragma once

#include "engine/scene/layers/LayerTime.hpp"
#include "engine/scene/routes/Route.hpp"

namespace groute {
    /**
     * @brief ReboundPath moves a Transform away from the target, then slingshots to the target.
     */
    class Rebound final : public geng::Route {
    public:
        /// Regular constructor
        Rebound(geng::Gear* gear, const geng::Vertex &target, float speed);
        /// Update override (true if done)
        bool update(geng::LayerTime& time) override;
    };
}