#pragma once



#include "../../world/GameWorld.hpp"

namespace geng {
    class WorldLoader {
    public:
        static GameWorld read_world(hstring filename);
    };
}
