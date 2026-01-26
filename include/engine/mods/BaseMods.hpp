#pragma once
#include "ModuleBundle.hpp"
#include "animation/Animator.hpp"
#include "banners/BannerModule.hpp"
#include "events/EventModule.hpp"

namespace gan {
    /// The standard bundle which when added to a layer provides
    /// Event functionally, Animation functionality, and Banner functionality.
    inline ModuleBundle<Events, Anim, Banners> std_mods;
}
