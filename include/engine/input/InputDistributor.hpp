#pragma once

#include <SDL.h>

#include "InputRouter.hpp"
#include "engine/EngineContext.hpp"
#include "engine/layers/Layer.hpp"

namespace geng {
    /** The InputDistributor handles sending input to the specified layer and ensuring that inputs are interpreted correctly.
     * @details the InputManager directly recieves keyboard polls and distributes the inputs accordingly. It has a */
    class InputDistributor final {
    private:
        /// Contains the engine_context context
        EngineContext& world;
        /// Contains all the InputRouters
        std::vector<InputRouter*> routers;
        /// Contains a map with all the states
        uint8_t* key_state;

    public:
        /// Constructor
        explicit InputDistributor(EngineContext& e);
        /// Update with a given key event & Layer. We use Layer Pointers because a layer might not be set up immedaitely.
        void process_event(const SDL_Event & e, Layer* active_layer) const;
        /// Updates all the input routers with the currently held events.
        void update(Layer *active_layer);

        /// Adds an input router
        void add_input_router(InputRouter* router);
        /// Removes an input router
        void remove_input_router(InputRouter* router);
        /// Adds the key_state pointer after initialization
        void _init();
    };
}
