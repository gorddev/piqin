#pragma once
#include "engine/scene/particles/ParticleGroup.hpp"
#include <deque>

#include "../../../layers/layer-subobjects/LayerState.hpp"

namespace gfx {

    struct RhombusInst {
        gan::vec2 pos;
        gan::vec2 velocity;
        short radius;
        float duration = 1500;
        RhombusInst(const gan::vec2 &offset, float speed, float size);
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
        Rhombus(gan::vec2 pos, float size, float speed, float duration, float frequency, SDL_Color color = {gan::max_alpha,gan::max_alpha,gan::max_alpha,gan::max_alpha});
        Rhombus(Gear* o, float size, float speed, float duration, float period, SDL_Color color = {gan::max_alpha,gan::max_alpha,gan::max_alpha,gan::max_alpha});

        bool update(gan::LayerState &time) override;
        void to_vertex(gan::RenderBuffer& buffer) override;
    };
}
