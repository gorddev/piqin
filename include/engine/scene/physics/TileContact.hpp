#pragma once

#include "engine/types/positioning/FPos2D.hpp"

namespace gan {
    /** Used for sending information between colliders and the physicsmaestro**/
    struct ProposedCollision {
        FPos2D newPos;
        FPos2D delta;
        uint16_t tile_type;
        bool collision;
    };
}
