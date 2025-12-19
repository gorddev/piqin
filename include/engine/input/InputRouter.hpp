#pragma once

#include <SDL.h>
#include "../scene/layers/Layer.hpp"

namespace geng {
    /**
     * @brief High-level input interception interface.
     * Routers receive semantic input callbacks (press, release, click).
     * Returning true consumes the input and prevents propagation.
     */
    struct InputRouter {
        InputRouter() = default;

        virtual ~InputRouter() = default;

        /** @return true if input is consumed */
        virtual bool get_press(SDL_Scancode key, Layer* active_layer) { return false; }

        /** @return true if input is consumed */
        virtual bool get_release(SDL_Scancode key, Layer* active_layer) { return false; }

        /** @return true if input is consumed */
        virtual bool get_click(Uint8 button, int x, int y, Layer* active_layer) { return false; }

        /** @return true if input is consumed */
        virtual bool get_click_release(Uint8 button, int x, int y, Layer* active_layer) { return false; }

        virtual bool get_scroll(int dx, int dy, Layer* active_layer) { return false; }
        virtual bool get_mouse_move(int x, int y, Layer* active_layer) { return false; }
    };
}