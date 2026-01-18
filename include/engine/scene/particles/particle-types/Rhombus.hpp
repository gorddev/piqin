#pragma once
#include "engine/scene/particles/ParticleGroup.hpp"
#include <deque>

#include "engine/layers/LayerState.hpp"

namespace gfx {

    struct RhombusInst {
        gan::FPos2D pos;
        gan::FPos2D velocity;
        short radius;
        float duration = 1500;
        RhombusInst(const gan::FPos2D &offset, float speed, float size);
        // Updates rhombus position
        bool update(double& dt);
        void to_vertex(gan::RenderBuffer &buffer, SDL_Color &color) const;
    };

    class Rhombus final : public gan::ParticleGroup {
    private:
        std::deque<RhombusInst> particles;
        float deltat = 0;
        float period;
    public:
        Rhombus(gan::FPos2D pos, float size, float speed, float duration, float frequency, SDL_Color color = {255,255,255,255});
        Rhombus(Gear* o, float size, float speed, float duration, float period, SDL_Color color = {255,255,255,255});

        bool update(gan::LayerState &time) override;
        void to_vertex(gan::RenderBuffer& buffer) override;
    };
}
