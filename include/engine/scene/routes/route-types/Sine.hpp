#pragma once

#include "engine/layers/LayerState.hpp"
#include "engine/scene/routes/Route.hpp"

namespace groute {
    /**
     * @brief SinePath moves an object's x linearly, and has it y follow a sin wave.
     */
    class Sine final : public gan::Route {
    private:
        /// Direction of travel for the path.
        gan::FPos2D direction;
    public:
        /// Regular constructor
        Sine(gan::Gear& gear, const gan::FPos2D &target, float speed);
        /// Update override (true if done)
        bool update(gan::LayerState& time) override;
    };
}