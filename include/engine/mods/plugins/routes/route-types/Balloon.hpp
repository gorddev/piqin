#pragma once

#include "../../../layers/layer-subobjects/LayerState.hpp"
#include "engine/mods/routes/Route.hpp"

namespace groute {
    /**
     * @brief Balloon path starts moving a Transform2D very fast, and then slows down as it approaches the target.
     */
    class Balloon final : public gan::Route {
    private:
        gan::vec2 direction;
    public:
        /// Regular constructor
        Balloon(gan::Gear& gear, const gan::vec2 &target, float speed);
        /// Update override (true if done)
        bool update(gan::LayerState& time) override;
    };
}