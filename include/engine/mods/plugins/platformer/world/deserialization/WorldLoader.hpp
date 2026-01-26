#pragma once



#include "../../world/GameWorld.hpp"

namespace gan {
    class WorldLoader {
    public:
        static GameWorld read_world(hstring filename);
    };
}
