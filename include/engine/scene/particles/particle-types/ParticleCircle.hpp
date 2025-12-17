#pragma once
#include <deque>
#include <SDL_rect.h>

#include "engine/particles/ParticleGroup.hpp"

namespace geng {

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
        ParticleCircle(Actor* a, float size, float speed, float duration, float period, SDL_Color Tint = {0,0,0,30});

        bool update(double& dt) override;
        std::vector<std::vector<SDL_FRect>> to_vertex() override;
    };

}
