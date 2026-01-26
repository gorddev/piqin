#pragma once

#include "engine/types/positioning/vec2.hpp"

namespace gan {
    /** Used for sending information between colliders and the physicsmaestro**/
    struct ProposedCollision {
        vec2 newPos;
        vec2 delta;
        uint16_t tile_type;
        bool collision;
    };
}
