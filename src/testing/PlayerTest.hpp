#pragma once
#include "engine/scene/physics/Collider.hpp"

class PlayerTest : public geng::Collider{
protected:
    bool grounded = false;
public:
    PlayerTest(geng::FrameTable& frames) : Collider(frames, {10,10}) {

    }
};
