#pragma once

#include "engine/scene/layers/LayerTime.hpp"
#include "engine/scene/routes/Route.hpp"

namespace groute {
    /**
     * @brief SinePath moves an object's x linearly, and has it y follow a sin wave.
     */
    class Sine final : public geng::Route {
    private:
        /// Direction of travel for the path.
        geng::Vertex direction;
    public:
        /// Regular constructor
        Sine(geng::Gear* gear, const geng::Vertex &target, float speed);
        /// Update override (true if done)
        bool update(geng::LayerTime& time) override;
    };
}