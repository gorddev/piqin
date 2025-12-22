#pragma once

#include <string>

#include "../../world/GameWorld.hpp"

namespace geng {
    class WorldLoader {
    public:
        static GameWorld read_world(const std::string& filename);
    };
}
