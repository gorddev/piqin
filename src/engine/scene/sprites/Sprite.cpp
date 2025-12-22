#include "engine/scene/sprites/Sprite.hpp"

using namespace geng;

Sprite::Sprite(FrameTable &frames)
    : frameTable(frames), anim (frames.get_frame(0), 0) {
    texture_id = frames.get_texture_id();
}

/// Default Constructor
Sprite::Sprite(FrameTable& frames, uint16_t default_animation)
    : frameTable(frames),
      anim(frames.get_frame(default_animation), default_animation) {
    anim.set_frame(frames.get_frame(default_animation, 0));
    set_shadow();
    texture_id = frames.get_texture_id();
}
/// Defines an Sprite with a starting position
Sprite::Sprite(FrameTable& frames,Transform2D t, uint16_t default_animation)
    : Gear(t),
      frameTable(frames),
      anim(frames.get_frame(default_animation), default_animation) {
    set_shadow();
    texture_id = frames.get_texture_id();
}

/// Non-virtual update_frame function.
void Sprite::update_frame(LayerState& time) {
    if (anim.update(time))
        frameTable.update_frame(anim);
}

/// To String
std::string Sprite::to_string() const {
    return "Sprite:" + Gear::to_string() + " " + t.to_string() + "\n" + anim.to_string();
}

/// Converts an sprite to buffer
void Sprite::to_vertex(RenderBuffer &buffer) {
    anim.calc_vertices(buffer, this);
    if (has_shadow())
        buffer.push_shadow(6);
}

