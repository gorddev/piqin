#pragma once

#include "../core/EngineContext.hpp"

namespace geng {

    /** Only allows the engine to make an InputRouter **/
    class InputRouterInit {
        explicit InputRouterInit(EngineContext& core, uint8_t*& keystates) : core(core), key_states(keystates) {}
        friend class Engine;
        friend class InputRouter;
        EngineContext& core;
        uint8_t* key_states;
    };
}
