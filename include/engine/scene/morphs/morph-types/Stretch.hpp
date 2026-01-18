#pragma once

#include "engine/layers/LayerState.hpp"
#include "engine/scene/morphs/Morph.hpp"

namespace gfx {

    /** Stretch is a permanent morph that stretches an object when it moves. **/
    class Stretch final : public gan::Morph{
    private:
        gan::FPos2D oldPos;
        float stretchX = 1.f;
        float stretchY = 1.f;
        float snappiness = 1.f;
        float tolerance = 1.f;
    public:
        /// Snappiness specifies how fast the object stretches.
        Stretch(gan::Gear* gear, float amplitude, float snappiness = 1.f, float tolerance = 1.f);

        /// Override function.
        bool update(gan::LayerState &time) override;
    };
}
