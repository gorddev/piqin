#pragma once

#include "engine/scene/layers/LayerTime.hpp"
#include "engine/scene/routes/Route.hpp"

namespace groute {
    /**
     * @brief TorpedoPath starts an object moving slowly, and then speeds it up as it moves along.
     */
    class Torpedo final : public geng::Route {
    private:
        /// Direction of travel for the path.
        geng::Vertex direction;
    public:
        /// Regular constructor
        Torpedo(geng::Gear* gear, const geng::Vertex &target, float speed);
        /// Update override (true if done)
        bool update(geng::LayerTime& time) override;
    };
}
