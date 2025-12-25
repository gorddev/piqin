#pragma once

#include "engine/EngineContext.hpp"
#include "engine/scene/initializer/TextureRegister.hpp"

namespace geng {

    /**
     * Provides all the information necessary to initialize a Layer,
     * such as core, camera, TextureRegister, etc.
     */
    class LayerInit {
    private:
        /// Private constructor only accessible by the engine.
        LayerInit(EngineContext& context, const Camera& camera, TextureRegister& texreg)
            : core(context), camera(camera), texreg(texreg) {}
        /// Hello friend!
        friend class Engine;
        friend class Layer;
        /// Member vars
        EngineContext& core;
        const Camera& camera;
        TextureRegister& texreg;
    };
}
