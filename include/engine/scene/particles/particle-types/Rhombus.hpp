#pragma once
#include "engine/scene/particles/ParticleGroup.hpp"
#include <deque>

#include "engine/layers/LayerState.hpp"

namespace gfx {

    struct RhombusInst {
        geng::FPos2D pos;
        geng::FPos2D velocity;
        short radius;
        float duration = 1500;
        RhombusInst(const geng::FPos2D &offset, float speed, float size);
        // Updates rhombus position
        bool update(double& dt);
        void to_vertex(geng::RenderBuffer &buffer, SDL_Color &color) const;
    };

    class Rhombus final : public geng::ParticleGroup {
    private:
        std::deque<RhombusInst> particles;
        float deltat = 0;
        float period;
    public:
        Rhombus(geng::FPos2D pos, float size, float speed, float duration, float frequency, SDL_Color color = {255,255,255,255});
        Rhombus(Gear* o, float size, float speed, float duration, float period, SDL_Color color = {255,255,255,255});

        bool update(geng::LayerState &time) override;
        void to_vertex(geng::RenderBuffer& buffer) override;
    };
}
