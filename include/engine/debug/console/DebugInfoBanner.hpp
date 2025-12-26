#pragma once

#include "engine/layers/Layer.hpp"
#include "engine/scene/banners/Banner.hpp"
#include "../debug-utilities/DebugText.hpp"

namespace geng::debug {

    /** The Console can be used to print out text and the like **/
    class DebugInfoBanner final : public Banner {
    public:
        /// Holds the top banner info
        Banner info;
        /// Holds the displayed text
        DebugText<800> text;
        /// Contains the currently selected gear to hold information storage on.
        Gear* selected = nullptr;

    public:
        explicit DebugInfoBanner();

        void update(Layer*& active_layer, DebugContext& debugger);
        void to_vertex(RenderBuffer& buffer);
        bool get_press(SDL_Scancode key, Layer*& active_layer);
    };

} // namespace geng::debug