#pragma once
#include "engine/types/Gear.hpp"

namespace geng {

    class Banner : public geng::Gear {
    private:
        /// Texture id that contains all elements needed to render a banner.
        int texture_id = -1;

    };
}
