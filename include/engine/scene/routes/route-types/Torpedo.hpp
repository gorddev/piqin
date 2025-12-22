#pragma once

#include "engine/layers/LayerState.hpp"
#include "engine/scene/routes/Route.hpp"

namespace groute {
    /**
     * @brief TorpedoPath starts an object moving slowly, and then speeds it up as it moves along.
     */
    class Torpedo final : public geng::Route {
    private:
        /// Direction of travel for the path.
        geng::FPos2D direction;
    public:
        /// Regular constructor
        Torpedo(geng::Gear& gear, const geng::FPos2D &target, float speed);
        /// Update override (true if done)
        bool update(geng::LayerState& time) override;
    };
}
