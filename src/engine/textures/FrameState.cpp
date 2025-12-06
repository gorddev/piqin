#include "engine/textures/FrameState.hpp"

#include "engine/gengine-globals/scene.hpp"

using namespace gengine;

// .............
// Functions for Sprite
// ..............

bool FrameState::update() {
    if (frameType != GENG_Anim::IDLE || dirty) {
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


std::string FrameState::to_string() {
    return std::string("FrameState { ") +
        "frame=" + std::to_string(reinterpret_cast<uintptr_t>(frame)) + ", " +
        "duration=" + std::to_string(duration) + ", " +
        "texture_id=" + std::to_string(texture_id) + ", " +
        "frame_sheet_id=" + std::to_string(frame_sheet_id) + ", " +
        "animation_index=" + std::to_string(animation_index) + ", " +
        "frame_index=" + std::to_string(frame_index) + ", " +
        "default_animation=" + std::to_string(default_animation) + ", " +
        "frameType=" + std::to_string(static_cast<int>(frameType)) + ", " +
        "dirty=" + (dirty ? "true" : "false") +
        " }";
}