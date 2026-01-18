#pragma once

#include "../core/EngineContext.hpp"
#include "engine/scene/initializer/TextureRegister.hpp"

namespace gan {

    /**
     * Provides all the information necessary to initialize a Layer,
     * such as core, camera, TextureRegister, etc.
     */
    class LayerInit {
    private:
        /// Private constructor only accessible by the engine.
        LayerInit(EngineContext& context, TextureRegister& texreg)
            : core(context), texreg(texreg) {}
        /// Hello friend!
        friend class Engine;
        friend class Layer;
        /// Member vars
        EngineContext& core;
        TextureRegister& texreg;
    };
}
