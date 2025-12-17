#include "engine/scene/animation/AnimInfo.hpp"

#include "engine/scene/layers/LayerTime.hpp"

using namespace geng;

// .............
// Functions for FrameState
// ..............

AnimInfo::AnimInfo(uint16_t texture_id, uint16_t frame_sheet_id, uint16_t animation_index)
    : texture_id(texture_id), frame_table_id(frame_sheet_id), animation_index(animation_index) {}

bool AnimInfo::update(LayerTime& time) {
    if (frameType != GAnimType::IDLE || dirty) {
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

uint16_t AnimInfo::get_frame_table_id() const {
    return frame_table_id;
}

uint16_t AnimInfo::get_texture_id() const {
    return texture_id;
}

void AnimInfo::set_frame(Frame& s) {
    frame = &s;
    dirty = false;
    frameType = s.get_anim();
    duration += s.get_duration();
}

void AnimInfo::set_animation(uint16_t new_animation) {
    dirty = true;
    animation_index = new_animation;
    frame_index = 0;
    duration = 0.0f;
}

void AnimInfo::set_frame_table_id(
    uint16_t new_frame_table_id) {
    frame_table_id = new_frame_table_id;
}

void AnimInfo::calc_vertices(RenderBuffer &buffer, Gear* gear) {
    frame->append_vertices(buffer, gear);
}

int AnimInfo::pre_increment_frame() {
    return ++frame_index;
}


std::string AnimInfo::to_string() const {
    return std::string("AnimInfo { ") +
        "frame=" + std::to_string(reinterpret_cast<uintptr_t>(frame)) + ", " +
        "duration=" + std::to_string(duration) + ", " +
        "texture_id=" + std::to_string(texture_id) + ", " +
        "frame_table_id=" + std::to_string(frame_table_id) + ", " +
        "animation_index=" + std::to_string(animation_index) + ", " +
        "frame_index=" + std::to_string(frame_index) + ", " +
        "default_animation=" + std::to_string(default_animation) + ", " +
        "frameType=" + std::to_string(static_cast<int>(frameType)) + ", " +
        "dirty=" + (dirty ? "true" : "false") +
        " }";
}
