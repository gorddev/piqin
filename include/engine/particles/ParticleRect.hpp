#pragma once
#include "ParticleGroup.hpp"

namespace gengine {

    struct PRect {
        Vertex pos;
        float duration;
        float initDuration;
        short gerth;
        short width;
        short height;
        PRect (Vertex pos, float duration, short gerth, short width, short height);
        bool update();
        [[nodiscard]] std::vector<SDL_FRect> to_rect(float& speed) const;
    };

    class ParticleRect final : public ParticleGroup {
        std::deque<PRect> particles;
        float deltat = 0;
        float period;
        short width, height;
    public:
        ParticleRect(Vertex pos, float size, float speed, float duration, float period, short width, short height, SDL_Color Tint = {0,0,0,30});
        ParticleRect(Object* o, float size, float speed, float duration, float period, short width, short height, SDL_Color Tint = {0,0,0,30});

        bool update() override;
        std::vector<std::vector<SDL_FRect>> to_vertex() override;

    };
}
