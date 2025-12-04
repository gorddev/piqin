#pragma once
#include "Frame.hpp"
#include <functional>

namespace gengine {
    // Is a struct because we dont want to call functions for each var access
    struct FrameState {
        // Frame we are currently on
        textures::Frame* frame = nullptr;
        // Duration
        float duration = 0.f;
        // ID of the sprite's texture
        short texture_id = 0;
        // ID of the sprite's frameSheet
        short frame_sheet_id = 0;
        // The current animation the sprite is on
        short animation_index = 0;
        // The current frame of the animation the sprite is one
        short frame_index = 0;
        // The default animation for this sprite
        short default_animation = 0;
        // Lets us easily check frame type
        GENG_Anim frameType = GENG_Anim::IDLE;
        // Lets us check if we've changed our animation
        bool dirty = false;

        // We return the duration.
        bool update();
        // Applies all the parameters of a frame
        void set_frame(textures::Frame& s);
        // Sets the animation index.
    };
}
