#pragma once

#include "../../../layers/layer-subobjects/LayerState.hpp"
#include "engine/mods/routes/Route.hpp"

namespace groute {
    /**
     * @brief SinePath moves an object's x linearly, and has it y follow a sin wave.
     */
    class Sine final : public gan::Route {
    private:
        /// Direction of travel for the path.
        gan::vec2 direction;
    public:
        /// Regular constructor
        Sine(gan::Gear& gear, const gan::vec2 &target, float speed);
        /// Update override (true if done)
        bool update(gan::LayerState& time) override;
    };
}