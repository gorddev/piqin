#pragma once
#include <deque>
#include <SDL_rect.h>

#include "engine/particles/ParticleGroup.hpp"

namespace gan {

    struct PCircle {
        FPos2D pos;
        FPos2D direction;
        int radius;
        float duration;
        float initDuration;
        PCircle(FPos2D pos, int radius, float duration);
        bool update();
        [[nodiscard]] lni::vector<SDL_FRect> to_rect(float& speed);
    };

    class ParticleCircle : public ParticleGroup {
    private:
        std::deque<PCircle> particles;
        float deltat = 0;
        float period;
    public:
        ParticleCircle(FPos2D pos, float size, float speed, float duration, float frequency, SDL_Color Tint = {0,0,0,30});
        ParticleCircle(Sprite* a, float size, float speed, float duration, float period, SDL_Color Tint = {0,0,0,30});

        bool update(double& dt) override;
        lni::vector<lni::vector<SDL_FRect>> to_vertex() override;
    };

}
