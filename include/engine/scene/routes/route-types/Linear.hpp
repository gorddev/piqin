#pragma once

#include "engine/layers/LayerState.hpp"
#include "engine/scene/routes/Route.hpp"

namespace groute {
    /**
     * @brief LinearPath moves a Transform2D towards a destination
     * in a straight line at constant speed.
     */
    class Linear final : public geng::Route {
    private:
        geng::FPos2D direction;
    public:
        /// Regular constructor
        Linear(geng::Gear& gear, const geng::FPos2D &target, float speed);
        /// Update override (true if done)
        bool update(geng::LayerState& time) override;
    };
}