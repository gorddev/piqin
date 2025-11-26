#pragma once
#include "scene/render/Camera.hpp"
#include "scene/textures/SheetManager.hpp"
#include "scene/ObjectManager.hpp"
#include "input/InputManager.hpp"
#include "game/GameMaster.hpp"

namespace engine {
    inline SheetManager* sm = nullptr;
    inline ObjectManager* om = nullptr;
    inline InputManager* input = nullptr;
}

namespace game {
    inline Camera* camera = nullptr;
    inline Hand* hand = nullptr;
}