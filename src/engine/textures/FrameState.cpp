#include "engine/textures/FrameState.hpp"

#include "engine/gengine-globals/scene.hpp"

using namespace gengine;

// .............
// Functions for Sprite
// ..............

bool FrameState::update() {
    if (frameType != GENG_Anim::IDLE || !dirty) {
        duration -= glb::scene.dt;
        if (duration <= 0) {
            dirty = false;
            return true;
        }
    }
    return false;
}

void FrameState::set_frame(textures::Frame& s) {
    frame = &s;
    dirty = false;
    frameType = s.anim;
    duration = s.duration;
}
