#include "engine/scene/particles/particle-types/Rhombus.hpp"

#include "../../../../../include/engine/layers/layer-subobjects/LayerState.hpp"

using namespace gan;
using namespace gfx;

RhombusInst::RhombusInst(const vec2 &offset, float speed, float size) {
    pos = offset;
    velocity.randomize();
    velocity = velocity.unit();
    velocity *= speed;
    radius = size;
}

bool RhombusInst::update(double& dt) {
    duration -= dt;
    if (duration <= 0)
        return true;
    pos.x += velocity.x * dt* 0.05;
    pos.y += velocity.y * dt* 0.05;
    return false;
}

void RhombusInst::to_vertex(RenderBuffer& buffer, SDL_Color& color) const {
    // Radius
    float rad = (radius * duration / 1500.f);
    // FPos2D locations

    SDL_FPoint top = {pos.x, pos.y - rad};
    SDL_FPoint bottom = {pos.x, pos.y + rad};
    SDL_FPoint left = {pos.x - rad, pos.y};
    SDL_FPoint right = {pos.x + rad, pos.y};

    buffer.push_back(top, color);
    buffer.push_back(left, color);
    buffer.push_back(bottom, color);
    buffer.push_back(top, color);
    buffer.push_back(right, color);
    buffer.push_back(bottom, color);
}

// Rhombus constructors
Rhombus::Rhombus(vec2 pos, float size, float speed, float duration, float frequency, SDL_Color Tint)
    : ParticleGroup(pos, size, speed, duration, Tint), period(frequency) {
    color = Tint;
    t.color = Tint;
    if (duration == -1)
        permanent = true;
}

Rhombus::Rhombus(Gear* o, float size, float speed, float duration, float period, SDL_Color Tint)
    : ParticleGroup(o, size, speed, duration, Tint), period(period) {
    color = Tint;
    if (duration == -1)
        permanent = true;
}

bool Rhombus::update(LayerState &time) {
    // Check if we're done

    duration -= time.get_dt();
    deltat += time.get_dt();
    bool done = (duration <= 0) && !permanent;
    if (!done) {
        while (deltat > period) {

            if (payload != nullptr) {
                vec2 diff = (t.pos - payload->t.pos) * period/deltat;
                particles.emplace_back(payload->t.offset + payload->t.pos + diff, speed, strength);
            }
            else
                particles.emplace_back(vec2(0,0), speed, strength);
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

void Rhombus::to_vertex(RenderBuffer& buffer) {
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