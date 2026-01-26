#pragma once
#include "../ParticleGroup.hpp"
#include <deque>

#include "../../../layers/layer-subobjects/LayerState.hpp"

namespace gfx {

    struct SparkleInst {
        gan::vec2 pos;
        gan::vec2 velocity;
        short radius;
        float duration = 1500;
        SparkleInst(const gan::vec2 &offset, float speed, float size);
        // Updates rhombus position
        bool update(double& dt);
        void to_vertex(gan::RenderBuffer &buffer, SDL_Color &color);
    };

    class Sparkle : public gan::ParticleGroup {
    private:
        std::deque<SparkleInst> particles;
        float deltat = 0;
        float period;
    public:
        Sparkle(gan::vec2 pos, float size, float speed, float duration, float frequency, SDL_Color Tint = {0,0,0,30});
        Sparkle(Gear* o, float size, float speed, float duration, float period, SDL_Color Tint = {0,0,0,30});

        bool update(gan::LayerState &time) override;
        void to_vertex(gan::RenderBuffer &buffer) override;
    };
}
