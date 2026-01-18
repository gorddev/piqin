#include "engine/scene/particles/particle-types/RhombusRotate.hpp"

#include "engine/scene/particles/particle-types/Rhombus.hpp"

#include "engine/layers/LayerState.hpp"

using namespace gan;
using namespace gfx;

RhombusRotateInst::RhombusRotateInst(const FPos2D &offset, float speed, float size) {
    pos = offset;
    velocity.randomize();
    velocity = velocity.unit();
    velocity *= speed;
    radius = size;
}

bool RhombusRotateInst::update(double& dt) {
    duration -= dt;
    if (duration <= 0)
        return true;
    pos.x += velocity.x * dt* 0.05;
    pos.y += velocity.y * dt* 0.05;
    return false;
}

void RhombusRotateInst::to_vertex(RenderBuffer& buffer, SDL_Color& color) const {
    // Radius
    int rad = (radius * duration / 1500.f);
    // FPos2D locations

    float angle = duration / 100.f;
    float s = sinf(angle);
    float c = cosf(angle);

    SDL_FPoint top = {
        pos.x + rad * s,
        pos.y - rad * c
    };
    SDL_FPoint bottom = {
        pos.x - rad * s,
        pos.y + rad * c
    };
    SDL_FPoint left = {
        pos.x - rad * c,
        pos.y - rad * s
    };
    SDL_FPoint right = {
        pos.x + rad * c,
        pos.y + rad * s
    };

    buffer.push_back(top, color);
    buffer.push_back(left, color);
    buffer.push_back(bottom, color);
    buffer.push_back(top, color);
    buffer.push_back(right, color);
    buffer.push_back(bottom, color);
}

// Rhombus constructors
RhombusRotate::RhombusRotate(FPos2D pos, float size, float speed, float duration, float frequency, SDL_Color Tint)
    : ParticleGroup(pos, size, speed, duration, Tint), period(frequency) {
    color = Tint;
    t.color = Tint;
    if (duration == -1)
        permanent = true;
}

RhombusRotate::RhombusRotate(Gear* o, float size, float speed, float duration, float period, SDL_Color Tint)
    : ParticleGroup(o, size, speed, duration, Tint), period(period) {
    color = Tint;
    if (duration == -1)
        permanent = true;
}

bool RhombusRotate::update(LayerState &time) {
    // Check if we're done

    duration -= time.get_dt();
    deltat += time.get_dt();
    bool done = (duration <= 0) && !permanent;
    if (!done) {
        while (deltat > period) {

            if (payload != nullptr) {
                FPos2D diff = (t.pos - payload->t.pos) * period/deltat;
                particles.emplace_back(payload->t.offset + payload->t.pos + diff*5, speed, strength);
            }
            else
                particles.emplace_back(FPos2D(0,0), speed, strength);
            deltat -= period;
        }
    }
    double dt = time.get_dt();
    for (auto it = particles.begin(); it != particles.end();) {
        if (it->duration <= 0) {
            it = particles.erase(it);
        }
        else {
            done = false;
            it->update(dt);
            ++it;
        }
    }
    return done;
}

void RhombusRotate::to_vertex(RenderBuffer& buffer) {
    int count = 0;
    if (payload != nullptr)
        t.pos = payload->t.pos;
    for (auto& i : particles) {
        buffer.begin_object();
        i.to_vertex(buffer, t.color);
        count+=6;
        buffer.push_shadow(6);
        buffer.end_object();
    }
    /*
    if (shadow())
        get_shadow_calc().apply_shadow(buffer, count);
        */
}