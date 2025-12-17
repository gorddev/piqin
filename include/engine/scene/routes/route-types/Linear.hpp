#pragma once

#include "engine/scene/layers/LayerTime.hpp"
#include "engine/scene/routes/Route.hpp"

namespace groute {
    /**
     * @brief LinearPath moves a Transform towards a destination
     * in a straight line at constant speed.
     */
    class Linear final : public geng::Route {
    private:
        geng::Vertex direction;
    public:
        /// Regular constructor
        Linear(geng::Gear* gear, const geng::Vertex &target, float speed);
        /// Update override (true if done)
        bool update(geng::LayerTime& time) override;
    };
}