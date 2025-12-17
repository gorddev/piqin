#pragma once

#include "engine/scene/layers/LayerTime.hpp"
#include "engine/scene/routes/Route.hpp"

namespace groute {
    /**
     * @brief Balloon path starts moving a Transform very fast, and then slows down as it approaches the target.
     */
    class Balloon final : public geng::Route {
    private:
        geng::Vertex direction;
    public:
        /// Regular constructor
        Balloon(geng::Gear* gear, const geng::Vertex &target, float speed);
        /// Update override (true if done)
        bool update(geng::LayerTime& time) override;
    };
}