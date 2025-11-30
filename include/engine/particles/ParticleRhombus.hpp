#pragma once
#include "ParticleGroup.hpp"

namespace gengine {
    class ParticleRhombus : public ParticleGroup {
    private:
        std::vector<std::vector<SDL_Rect>> particles;
    public:
        ParticleRhombus(float size, float speed, float duration);
    };
}