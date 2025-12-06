#pragma once
#include "ParticleGroup.hpp"
#include <deque>

namespace gengine {

    struct Sparkle {
        Vertex pos;
        Vertex velocity;
        short radius;
        float duration = 1500;
        Sparkle(const Vertex &offset, float speed, float size);
        // Updates rhombus position
        bool update();
        void to_vertex(std::vector<SDL_Vertex>& buffer, SDL_Color& color);
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
        int to_vertex(std::vector<SDL_Vertex>& buffer) override;
    };
}
