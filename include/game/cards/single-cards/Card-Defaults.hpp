#pragma once
#include "engine/types/Transform2D.hpp"
#include "game/initialization/initialization_defaults.hpp"

namespace card {

    const gan::Transform2D default_card_transform = {
        {0,0},
        20, 29, init::default_card_scale
    };

    const int flipped_card_anim = 55;
}
