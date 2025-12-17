#pragma once

#include <SDL.h>

#include "InputRouter.hpp"
#include "engine/EngineContext.hpp"
#include "engine/scene/Layer.hpp"

namespace geng {
    /** The InputSource handles sending input to the specified layer and ensuring that inputs are interpreted correctly.
     * @details the InputManager directly recieves keyboard polls and distributes the inputs accordingly. It has a */
    class InputSource final {
    private:
        /// Contains the world context
        EngineContext& world;
        /// Contains all the InputRouters
        std::vector<InputRouter*> routers;
    public:
        // Constructor
        explicit InputSource(EngineContext& e);
        // Update with a given key event & Layer
        void update(const SDL_Event & e, Layer* active_layer) const;
        // Adds an input router
        void add_input_router(InputRouter* router);
        // Removes an input router
        void remove_input_router(InputRouter* router);
    };
}
