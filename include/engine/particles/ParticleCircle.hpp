#pragma once
#include <deque>
#include <SDL_rect.h>

#include "engine/particles/ParticleGroup.hpp"
#include "engine/utilities/types/Vertex.hpp"

namespace gengine {

    struct PCircle {
        Vertex pos;
        Vertex direction;
        int radius;
        float duration;
        float initDuration;
        PCircle(Vertex pos, int radius, float duration);
        bool update();
        [[nodiscard]] std::vector<SDL_FRect> to_rect(float& speed);
    };

    class ParticleCircle : public ParticleGroup {
    private:
        std::deque<PCircle> particles;
        float deltat = 0;
        float period;
    public:
        ParticleCircle(Vertex pos, float size, float speed, float duration, float frequency, SDL_Color Tint = {0,0,0,30});
        ParticleCircle(Object* o, float size, float speed, float duration, float period, SDL_Color Tint = {0,0,0,30});

        bool update() override;
        std::vector<std::vector<SDL_FRect>> to_rect() override;
    };

}
