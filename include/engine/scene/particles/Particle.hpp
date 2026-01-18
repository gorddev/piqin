#pragma once
#include "engine/types/positioning/FPos2D.hpp"

namespace gan {

    /** Single Particle that can be used to create may beautiful things */
    struct Particle {
        /// Position of the particle
        FPos2D position;
        /// Velocity of the particle
        FPos2D velocity;
        /// Duration of the particle
        float duration = -1;
        /// Ties the particle to a specific group for updating purposes
        short group_id = -1;
        /// Ties the particle to a specific shape.
        short shape_id = -1;
    };
}
