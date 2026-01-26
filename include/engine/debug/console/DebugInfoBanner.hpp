#pragma once

#include "engine/layers/Layer.hpp"
#include "engine/mods/banners/Banner.hpp"
#include "../debug-utilities/DebugText.hpp"
#include "engine/types/cqueue/cqueue.hpp"

namespace gan::debug {

    /** The Console can be used to print out text and the like **/
    class DebugInfoBanner final : public Banner {
    public:
        /// Holds the top banner info
        Banner info;
        /// Holds the displayed text
        DebugText text;
        /// Contains the currently selected gear to hold information storage on.
        Gear* selected = nullptr;
        /// Contains a reference to the core for fonts
        EngineContext& core;
        /// Circular queue for fps
        cqueue<float, 10> fps_tracker;


    public:
            explicit DebugInfoBanner(EngineContext& core);

        void update(Layer*& active_layer, DebugContext& debugger);
        void to_vertex(RenderBuffer& buffer);
        bool get_press(SDL_Scancode key, Layer*& active_layer);
    };

} // namespace gan::debug