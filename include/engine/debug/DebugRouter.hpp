#pragma once
#include "engine/input/InputRouter.hpp"

namespace geng::debug {

    /** Allows for easily toggling of debug features **/
    class DebugRouter : public InputRouter {
    private:
        Debugger& debugger;
    public:
        explicit DebugRouter(InputRouterInit& init) : InputRouter(init), debugger(core.debugger) {}

        bool update(Layer*& active_layer) override {
            return false;
        }

        bool get_press(SDL_Scancode key, Layer *&active_layer) override {
            if (key == SDL_SCANCODE_F1) {
                debugger.debug_mode = !debugger.debug_mode;
            }
            else if (key == SDL_SCANCODE_F2) {
                debugger.toggle_hitboxes();
            }
            else if (key == SDL_SCANCODE_F3) {
                debugger.toggle_grid();
            }
            else if (key == SDL_SCANCODE_F4) {
                core.set_layer_change("__next");
            }
            else if (key == SDL_SCANCODE_F5) {
                core.set_layer_change("__run");
            }
            else if (key == SDL_SCANCODE_F6) {
                core.set_layer_change("__visible");
            }
            return false;
        }

    };
}
