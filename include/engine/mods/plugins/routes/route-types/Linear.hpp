#pragma once

#include "../../../layers/layer-subobjects/LayerState.hpp"
#include "engine/mods/routes/Route.hpp"

namespace groute {
    /**
     * @brief LinearPath moves a Transform2D towards a destination
     * in a straight line at constant speed.
     */
    class Linear final : public gan::Route {
    private:
        gan::vec2 direction;
    public:
        /// Regular constructor
        Linear(gan::Gear& gear, const gan::vec2 &target, float speed);
        /// Update override (true if done)
        bool update(gan::LayerState& time) override;
    };
}