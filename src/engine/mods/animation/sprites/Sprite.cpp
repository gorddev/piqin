#include "engine/scene/sprites/Sprite.hpp"

using namespace gan;

Sprite::Sprite(FrameTable &frames)
    : anim (frames.get_frame(0), 0), frameTable(frames) {
    texture_id = frames.get_texture_id();
}

/// Default Constructor
Sprite::Sprite(FrameTable& frames, uint16_t default_animation)
    : anim(frames.get_frame(default_animation), default_animation),
      frameTable(frames) {
    anim.set_frame(frames.get_frame(default_animation, 0));
    set_shadow();
    texture_id = frames.get_texture_id();
}
/// Defines an Sprite with a starting position
Sprite::Sprite(FrameTable& frames,Transform2D t, uint16_t default_animation)
    : Gear(t),
      anim(frames.get_frame(default_animation), default_animation),
      frameTable(frames) {
    set_shadow();
    texture_id = frames.get_texture_id();
}

/// Non-virtual update_frame function.
void Sprite::update_frame(LayerState& time) {
    if (anim.update(time))
        frameTable.update_frame(anim);
}

/// To Fstring
gan::str_view& Sprite::to_fstring_verbose(gan::str_view& buffer) const {
    Gear::to_fstring_verbose(buffer) << "\n";
    return anim.to_fstring_verbose(buffer);
}

/// To Fstring
gan::str_view& Sprite::to_fstring(gan::str_view& buffer) const {
    Gear::to_fstring(buffer) << "\n";
    return anim.to_fstring(buffer);
}

/// Converts an sprite to buffer
void Sprite::to_vertex(RenderBuffer &buffer) {
    anim.calc_vertices(buffer, this);
    if (has_shadow())
        buffer.push_shadow(6);
}

