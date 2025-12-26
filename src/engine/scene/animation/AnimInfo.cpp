#include "engine/scene/animation/AnimInfo.hpp"

#include "engine/layers/LayerState.hpp"

using namespace geng;

// .............
// Functions for FrameState
// ..............

AnimInfo::AnimInfo(Frame& first_frame) : frame(&first_frame) {
    default_animation = 0;
    next_anim = -1;
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

short AnimInfo::get_next_anim() const {
    return next_anim;
}

void AnimInfo::set_next_anim(short new_anim) {
    next_anim = new_anim;
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
    duration = s.get_duration();
    next_anim = s.get_next_anim();
}

void AnimInfo::set_animation(uint16_t new_animation, bool priorityStatus) {
    dirty = true;
    this->priority = priorityStatus;
    animation_index = new_animation;
    frame_index = 0;
    duration = 0.0f;
}

void AnimInfo::suggest_animation(short new_animation) {
    next_anim = new_animation;
}

void AnimInfo::queue_animation(short new_animation) {
    queued_anim = new_animation;
}

void AnimInfo::set_priority(bool state) {
    priority = state;
}

bool AnimInfo::has_priority() {
    return priority;
}

void AnimInfo::set_default_animation(
    uint16_t new_default_animation) {
    default_animation = new_default_animation;
}

short AnimInfo::get_queued_anim() const {
    return queued_anim;
}


void AnimInfo::calc_vertices(RenderBuffer &buffer, Gear* gear) {
    frame->append_vertices(buffer, gear);
}

int AnimInfo::pre_increment_frame() {
    return ++frame_index;
}

geng::str_view& AnimInfo::to_fstring_verbose(geng::str_view &buffer) const {
    return buffer   << precision<2>() << "[t][AnimInfo][n] " <<  "\tdefault_anim: " << default_animation << "\tbasedur: " << frame->get_duration() <<"\tdirty: " << (dirty ? "true" : "false") << "\n"
                    << "dur: " << static_cast<int>(duration)/100.f << "  \tanim_id: " << get_anim_id() << "\t\tframe_id: " << frame_index << "\t" << "anim_type: [e]" << geng::to_string(frame->get_anim_type()) << "[n]";
}

str_view & AnimInfo::to_fstring(geng::str_view &buffer) const {
    return buffer << precision<1>() << "[t][AInfo][n]\tAnim: "
        << animation_index << "\tFrm: " << frame_index
        << "\tType:[e] " << geng::to_string(frame->get_anim_type()) << "[n]";
}
