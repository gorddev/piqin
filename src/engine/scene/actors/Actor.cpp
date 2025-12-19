#include "engine/scene/actors/Actor.hpp"

using namespace geng;

Actor::Actor(FrameTable &frames)
    : frameTable(frames), anim (frames.get_frame(0), 0){}

/// Default Constructor
Actor::Actor(FrameTable& frames, uint16_t default_animation)
    : frameTable(frames),
      anim(frames.get_frame(default_animation), default_animation) {
    anim.set_frame(frames.get_frame(default_animation, 0));
    set_shadow();
}

/// Defines an Actor with a starting position
Actor::Actor(FrameTable& frames, uint16_t default_animation, Transform t)
    : Gear(t),
      frameTable(frames),
      anim(frames.get_frame(default_animation), default_animation) {
    set_shadow();
}

/// Non-virtual update_frame function.
void Actor::update_frame(LayerTime& time) {
    if (anim.update(time))
        frameTable.update_frame(anim);
}

/// To String
std::string Actor::to_string() const {
    return "Actor:" + Gear::to_string() + " " + t.to_string() + "\n" + anim.to_string();
}

/// Converts an actor to buffer
void Actor::to_vertex(RenderBuffer &buffer) {
    anim.calc_vertices(buffer, this);
    if (has_shadow())
        buffer.push_shadow(6);
}

/// Returns the z-index of
float Actor::z_index() const {
    return t.pos.z;
}
