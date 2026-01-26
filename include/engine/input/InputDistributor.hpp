#pragma once

#include <SDL.h>

#include "InputRouter.hpp"
#include "engine/core/EngineContext.hpp"
#include "engine/layers/Layer.hpp"

namespace gan {
    /** The InputDistributor handles sending input to the specified layer and ensuring that inputs are interpreted correctly.
     * @details the InputManager directly recieves keyboard polls and distributes the inputs accordingly. It has a */
    class InputDistributor final {
    private:
        /// Contains the engine_context core
        EngineContext& world;
        /// Contains all the InputRouters
        std::vector<InputRouter*> routers;
        /// Contains a map with all the states
        uint8_t* key_states;

        friend class Engine;
        /// Gets the held keystates
        uint8_t*& get_keystates();

    public:
        /// Constructor
        explicit InputDistributor(EngineContext& e);
        /// Update with a given key event & Layer. We use Layer Pointers because a layer might not be set up immedaitely.
        void process_event(const SDL_Event &e, Layer *active_layer);
        /// Updates all the input routers with the currently held events.
        void update(Layer *active_layer);
        /// Updates the input routers after event updating
        void post_update(Layer* active_layer);

        /// Adds an input router
        void add_input_router(InputRouter* router);
        /// Removes an input router
        void remove_input_router(InputRouter* router);

        void emplace_key_states(Layer *layer) const;

        /// Adds the key_states pointer after initialization
        void _init();

    private:
        /// Called when mouse is clicked
        void event_mouse_click(const SDL_Event &e, Layer *&active_layer);
        /// Called when mouse is released
        void event_mouse_release(const SDL_Event &e, Layer *&active_layer);
        /// Cllas the appropriate functions to handle mouse motion.
        void event_mouse_motion(const SDL_Event &e, Layer *&active_layer);

        void event_finger_up(SDL_TouchFingerEvent finger,
                             Layer *&active_layer);

        /// Called when finger is down
        void event_finger_down(SDL_TouchFingerEvent finger, Layer *&active_layer);

        void event_finger_motion(
            SDL_TouchFingerEvent finger,
            Layer *&active_layer);

        void event_mouse_wheel(SDL_MouseWheelEvent e,
                               Layer *&active_layer);

        /// Called when finger is down
        void event_key_down(SDL_Scancode key, Layer *&active_layer);
        /// Called when key is up
        void event_key_up(SDL_Scancode key,Layer *&active_layer);
    };
}
