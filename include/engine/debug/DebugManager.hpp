#pragma once

#include "console/Console.hpp"
#include "console/DebugButtonPanel.hpp"
#include "console/DebugInfoBanner.hpp"
#include "engine/input/InputRouter.hpp"

namespace geng::debug {

    class DebugManager : public InputRouter {
    private:
        /// Holds the core
        EngineContext& core;
        /// Holds all the debugging info
        DebugContext& debugger;
        /// Contains the console for typing and the like.
        DebugInfoBanner info_banner;
        /// Contains the buttons on the right hand side
        DebugButtonPanel button_panel;

        /// Debug camera
        Camera cam;
        /// creates name for debug;
        fstring<10> name = "debug";

        /// Lets us set up an input handler?
        LayerContext scene;

        friend class Engine;
    public:
        /// Lets us have clickable buttons in the overlay
        InputHandler input_handler;

        explicit DebugManager(InputRouterInit& init, EngineContext& core)
            : InputRouter(init), debugger(core.debugger),
                core(core), scene(name, cam), button_panel(core)
                , input_handler(scene)
        {
            button_panel.add_mouse_acceptors(input_handler);
        }


        bool update(Layer*& active_layer) override {
            if (geng::debug_mode) {
                info_banner.update(active_layer, debugger);
                button_panel.notify_update();
            }
            return false;
        }

        void notify_screen_resolution_change(Dim2D new_resolution) {
            button_panel.notify_screen_resolution_change(new_resolution);
            cam.set_dimensions(new_resolution);
        }

        void to_vertex(RenderBuffer& buffer) {
            info_banner.to_vertex(buffer);
            button_panel.to_vertex(buffer);
        }

        bool get_key_press(SDL_Scancode key, Layer *&active_layer) override;
        bool get_mouse_click(Uint8 button, Pos2D mousepos, Layer *&active_layer) override;
        bool get_mouse_release(Uint8 button, Pos2D mousepos, Layer*& active_layer) override;
        bool get_mouse_move(Pos2D mousepos, FPos2D deltapos, Layer *&active_layer) override;

    private:
    };
}
