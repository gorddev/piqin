#pragma once

#include "engine/scene/layers/LayerTime.hpp"
#include "engine/scene/morphs/Morph.hpp"

namespace gfx {

    /** Stretch is a permanent morph that stretches an object when it moves. **/
    class Stretch final : public geng::Morph{
    private:
        geng::Vertex oldPos;
        float stretchX = 1.f;
        float stretchY = 1.f;
        float snappiness = 1.f;
        float tolerance = 1.f;
    public:
        /// Snappiness specifies how fast the object stretches.
        Stretch(geng::Gear* gear, float amplitude, float snappiness = 1.f, float tolerance = 1.f);

        /// Override function.
        bool update(geng::LayerTime &time) override;
    };
}
