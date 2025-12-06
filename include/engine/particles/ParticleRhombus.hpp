#pragma once
#include "ParticleGroup.hpp"
#include <deque>

namespace gengine {
    class Object;

    struct Rhombus {
        Vertex pos;
        Vertex velocity;
        short radius;
        float duration = 1500;
        Rhombus(const Vertex &offset, float speed, float size);
        // Updates rhombus position
        bool update();
        void to_vertex(std::vector<SDL_Vertex>& buffer, SDL_Color& color) const;
    };

    class ParticleRhombus final : public ParticleGroup {
    private:
        std::deque<Rhombus> particles;
        float deltat = 0;
        float period;
    public:
        ParticleRhombus(Vertex pos, float size, float speed, float duration, float frequency, SDL_Color Tint = {0,0,0,30});
        ParticleRhombus(Object* o, float size, float speed, float duration, float period, SDL_Color Tint = {0,0,0,30});

        bool update() override;
        [[nodiscard]] int to_vertex(std::vector<SDL_Vertex>& buffer) override;
    };
}
