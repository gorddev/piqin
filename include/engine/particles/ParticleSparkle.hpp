#pragma once
#include "ParticleGroup.hpp"
#include <deque>

namespace gengine {
    class Object;

    struct Sparkle {
        Vertex pos;
        Vertex velocity;
        short radius;
        float duration = 1500;
        Sparkle(const Vertex &offset, float speed, float size);
        // Updates rhombus position
        bool update();
        [[nodiscard]] std::vector<SDL_FRect> to_rect() const;
    };

    class ParticleSparkle : public ParticleGroup {
    private:
        std::deque<Sparkle> particles;
        float deltat = 0;
        float period;
    public:
        ParticleSparkle(Vertex pos, float size, float speed, float duration, float frequency, SDL_Color Tint = {0,0,0,30});
        ParticleSparkle(Object* o, float size, float speed, float duration, float period, SDL_Color Tint = {0,0,0,30});

        bool update() override;
        std::vector<std::vector<SDL_FRect>> to_rect() override;
    };
}
