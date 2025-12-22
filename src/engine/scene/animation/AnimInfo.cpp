#include "engine/scene/animation/AnimInfo.hpp"

#include "engine/layers/LayerState.hpp"

using namespace geng;

// .............
// Functions for FrameState
// ..............

AnimInfo::AnimInfo(Frame& first_frame) : frame(&first_frame) {
    default_animation = 0;
    duration = frame->get_duration();
}

AnimInfo::AnimInfo(Frame& first_frame, uint16_t default_animation)
        : frame(&first_frame), default_animation(default_animation) {
    duration = frame->get_duration();
}

bool AnimInfo::update(LayerState& time) {
    if (frame->get_anim_type() != GAnimType::IDLE || dirty) {
        duration -= time.get_dt();
        if (duration <= 0) {
            dirty = true;
            return true;
        }
    }
    return false;
}

uint16_t AnimInfo::get_anim_id() const {
    return animation_index;
}

uint16_t AnimInfo::get_frame_index() const {
    return frame_index;
}

GAnimType AnimInfo::get_frame_type() const {
    return frame->get_anim_type();
}

uint16_t AnimInfo::get_default_animation() const {
    return default_animation;
}

void AnimInfo::set_frame_id(uint16_t new_frame_id) {
    frame_index = new_frame_id;
}

void AnimInfo::set_frame(Frame& s) {
    frame = &s;
    dirty = false;
    duration += s.get_duration();
}

void AnimInfo::set_animation(uint16_t new_animation) {
    dirty = true;
    animation_index = new_animation;
    duration = 0.0f;
}

void AnimInfo::set_default_animation(
    uint16_t new_default_animation) {
    default_animation = new_default_animation;
}


void AnimInfo::calc_vertices(RenderBuffer &buffer, Gear* gear) {
    frame->append_vertices(buffer, gear);
}

int AnimInfo::pre_increment_frame() {
    return ++frame_index;
}

std::string AnimInfo::to_string() const {
    return std::string("AnimInfo { ") +
        "duration=" + std::to_string(duration) + ", " +
        "animation_index=" + std::to_string(animation_index) + ", " +
        "frame_index=" + std::to_string(frame_index) + ", " +
        "default_animation=" + std::to_string(default_animation) + ", " +
        "dirty=" + (dirty ? "true" : "false") +
        " }";
}
