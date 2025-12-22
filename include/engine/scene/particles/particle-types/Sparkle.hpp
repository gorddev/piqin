#pragma once
#include "../ParticleGroup.hpp"
#include <deque>

#include "engine/layers/LayerState.hpp"

namespace gfx {

    struct SparkleInst {
        geng::FPos2D pos;
        geng::FPos2D velocity;
        short radius;
        float duration = 1500;
        SparkleInst(const geng::FPos2D &offset, float speed, float size);
        // Updates rhombus position
        bool update(double& dt);
        void to_vertex(geng::RenderBuffer &buffer, SDL_Color &color);
    };

    class Sparkle : public geng::ParticleGroup {
    private:
        std::deque<SparkleInst> particles;
        float deltat = 0;
        float period;
    public:
        Sparkle(geng::FPos2D pos, float size, float speed, float duration, float frequency, SDL_Color Tint = {0,0,0,30});
        Sparkle(Gear* o, float size, float speed, float duration, float period, SDL_Color Tint = {0,0,0,30});

        bool update(geng::LayerState &time) override;
        void to_vertex(geng::RenderBuffer &buffer) override;
    };
}
