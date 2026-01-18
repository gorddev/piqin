#pragma once

#include <SDL.h>

#include "InputRouterInit.hpp"
#include "engine/layers/Layer.hpp"

namespace gan {
    /**
     * @brief High-level input interception interface.
     *
     * @details
     * @code InputRouter@endcode provides overridable hooks for intercepting normalized input events
     * before they propagate to the active @code Layer@endcode. All callbacks return @code true@endcode
     * to consume input or @code false@endcode to allow propagation.
     * - -----------
     * - @code InputRouter()@endcode : Default constructor.
     * - @code virtual ~InputRouter()@endcode : Virtual destructor.
     * - ----- MUST OVERRIDE: --------
     * - @code virtual bool update(Layer*& active_layer) = 0@endcode : You decide what this does. It can just return false, but you must override it.
     * - Helpful: If you want keys down, use the @code is_held(SLD_Scancode key)@endcode function in @code InputRouter@endcode
     * - ------------ Input Callbacks (all @b virtual / overridable)
     * - @code virtual bool get_press(SDL_Scancode, Layer*)@endcode : Key press.
     * - @code virtual bool get_release(SDL_Scancode, Layer*)@endcode : Key release.
     * - @code virtual bool get_click(Uint8 button, int x, int y, Layer*)@endcode : Mouse button press.
     * - @code virtual bool get_click_release(Uint8 button, int x, int y, Layer*)@endcode : Mouse button release.
     * - @code virtual bool get_scroll(int dx, int dy, Layer*)@endcode : Scroll input.
     * - @code virtual bool get_mouse_move(int x, int y, Layer*)@endcode : Mouse movement.
     * @note
     * - All callbacks default to non-consuming behavior (@code false@endcode).
     */
    class InputRouter {
    private:
        /// Contains whether each key is held down or not
        uint8_t*& key_states;
    protected:
        /// Contains the engine core
        EngineContext& core;
        /// Default input router constructor
        InputRouter(InputRouterInit& init) : key_states(init.key_states), core(init.core) {}
        virtual ~InputRouter() = default;

        friend class Engine; // hey look a friend!!!!

    public:
        /// Accepts input if active
        bool active = true;

        /// @return @code true@endcode if the specified key is held down
        bool is_held(SDL_Scancode key) {
            if (key < 512 && key_states != nullptr)
                return key_states[key];
            return false;
        }

        /// @return True if you want to block all future input. Can just be a statement that returns false if you don't want to use it.
        virtual bool update(Layer*& active_layer, const uint8_t* keys) = 0;
        /** @return true if input is consumed */
        virtual bool get_key_press(SDL_Scancode key, Layer*& active_layer) { return false; }
        /** @return true if input is consumed */
        virtual bool get_key_release(SDL_Scancode key, Layer*& active_layer) { return false; }
        /** @return true if input is consumed */
        virtual bool get_mouse_click(Uint8 button, Pos2D mousepos, Layer*& active_layer) { return false; }
        /** @return true if input is consumed */
        virtual bool get_mouse_release(Uint8 button, Pos2D mousepos, Layer*& active_layer) { return false; }
        /** @return true if input is consumed */
        virtual bool get_mouse_scroll(SDL_MouseWheelEvent& e, Layer*& active_layer) { return false; }
        /** @return true if input is consumed */
        virtual bool get_finger_down(SDL_TouchFingerEvent finger, Layer*& active_layer) { return false; }
        /** @return true if input is consumed */
        virtual bool get_finger_release(SDL_TouchFingerEvent finger, Layer*& active_layer) { return false; }
        /** @return true if input is consumed */
        virtual bool get_finger_motion(SDL_TouchFingerEvent finger, Layer*& active_layer) { return false; }


        virtual bool get_scroll(int dx, int dy, Layer*& active_layer) { return false; }
        virtual bool get_mouse_move(Pos2D mousepos, FPos2D deltapos, Layer*& active_layer) { return false; }

    };
}