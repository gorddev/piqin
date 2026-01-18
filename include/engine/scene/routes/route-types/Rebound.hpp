#pragma once

#include "engine/layers/LayerState.hpp"
#include "engine/scene/routes/Route.hpp"

namespace groute {
    /**
     * @brief ReboundPath moves a Transform2D away from the target, then slingshots to the target.
     */
    class Rebound final : public gan::Route {
    public:
        /// Regular constructor
        Rebound(gan::Gear& gear, const gan::FPos2D &target, float speed);
        /// Update override (true if done)
        bool update(gan::LayerState& time) override;
    };
}