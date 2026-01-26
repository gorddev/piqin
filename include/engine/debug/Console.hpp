#pragma once

#include "console/ConsoleLogger.hpp"
#include "console/DebugButtonPanel.hpp"
#include "console/DebugInfoBanner.hpp"
#include "engine/input/InputRouter.hpp"

namespace gan {
    class Renderer;
}

namespace gan::debug {

    class Console : public InputRouter {
    private:
        /// Holds the core
        EngineContext& core;
        /// Holds all the debugging info
        DebugContext& debugger;

        // <><><> Renderable Elements
        /// Contains the console for typing and the like.
        DebugInfoBanner info_banner;
        /// Contains the buttons on the right hand side
        DebugButtonPanel button_panel;
        /// Contains the bottom console
        //ConsoleLogger logger;

        // <><><> Simulating a Layer <><><>
        /// creates name for debug;
        fstring<10> name = "__debug";
        /// Camera of the Console
        Camera cam = {0,0, 600,240};
        /// Lets us set up an InputHandler for clicking
        LayerCore scene;

        friend class Engine;
        friend class gan::Renderer;
    public:
        /// Lets us have clickable buttons in the overlay
        InputHandler input_handler;

        explicit Console(InputRouterInit& init, EngineContext& core)
            : InputRouter(init), core(core), debugger(core.debugger),
                info_banner(core), button_panel(core, cam)
                , scene(name, cam, core.width, core.height, nullptr, nullptr, core.tex_reg), input_handler(scene)
        {
            button_panel.add_mouse_acceptors(input_handler);
        }

        bool pre_update(Layer*& active_layer, const uint8_t* keys) override {
            if (gan::debug_mode) {
                info_banner.update(active_layer, debugger);
                button_panel.notify_update();
            }
            return false;
        }

        void notify_screen_resolution_change(dim2 new_resolution) {

            while (new_resolution.w/2 > 600) {
                new_resolution.w /= 2;
                new_resolution.h /= 2;
            }
            cam.set_dimensions(new_resolution);
            // Button_panel
            button_panel.notify_screen_resolution_change(cam.get_dimensions());
            // debuglogger
            //logger.notify_screen_resolution_change(cam.get_dimensions());
        }

        void to_vertex(RenderBuffer& buffer) {

            info_banner.to_vertex(buffer);
            button_panel.to_vertex(buffer);
            //logger.to_vertex(buffer);
        }

        bool get_key_press(SDL_Scancode key, Layer *&active_layer) override;
        bool get_mouse_click(Uint8 button, pos2 mousepos, Layer *&active_layer) override;
        bool get_mouse_release(Uint8 button, pos2 mousepos, Layer*& active_layer) override;
        bool get_mouse_move(pos2 mousepos, vec2 deltapos, Layer *&active_layer) override;

    };
}
