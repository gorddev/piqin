#pragma once

#include "engine/layers/layer-subobjects/LayerState.hpp"
#include "engine/mods/plugins/routes/Route.hpp"

namespace gfx {
    /**
     * @brief LinearPath moves a Transform2D towards a destination
     * in a straight line at constant speed.
     */
    class Linear final : public gfx::Route {
    private:
        gan::vec2 direction;
    public:
        /// Regular constructor
        Linear(gan::Gear& gear, const gan::vec2 &target, float speed);
        /// Update override (true if done)
        bool update(gan::LayerState& time) override;
    };
}