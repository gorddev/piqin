#pragma once

#include "engine/layers/layer-subobjects/LayerState.hpp"
#include "engine/mods/plugins/routes/Route.hpp"

namespace gfx {
    /**
     * @brief Balloon path starts moving a Transform2D very fast, and then slows down as it approaches the target.
     */
    class Balloon final : public gfx::Route {
    private:
        gan::vec2 direction;
    public:
        /// Regular constructor
        Balloon(gan::Gear& gear, const gan::vec2 &target, float speed);
        /// Update override (true if done)
        bool update(gan::LayerState& time) override;
    };
}