#pragma once

#include "engine/layers/LayerState.hpp"
#include "engine/scene/routes/Route.hpp"

namespace groute {
    /**
     * @brief Balloon path starts moving a Transform2D very fast, and then slows down as it approaches the target.
     */
    class Balloon final : public geng::Route {
    private:
        geng::FPos2D direction;
    public:
        /// Regular constructor
        Balloon(geng::Gear& gear, const geng::FPos2D &target, float speed);
        /// Update override (true if done)
        bool update(geng::LayerState& time) override;
    };
}