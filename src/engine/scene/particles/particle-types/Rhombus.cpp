#include "engine/scene/particles/particle-types/Rhombus.hpp"

#include <iostream>

#include "engine/scene/layers/LayerTime.hpp"

using namespace geng;
using namespace gfx;

RhombusInst::RhombusInst(const Vertex &offset, float speed, float size) {
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
    int rad = static_cast<int>(radius * duration / 1500.f);
    // Vertex locations

    SDL_FPoint top = {pos.x, pos.y - rad};
    SDL_FPoint bottom = {pos.x, pos.y + rad};
    SDL_FPoint left = {pos.x - rad, pos.y};
    SDL_FPoint right = {pos.x + rad, pos.y};

    buffer.push_back(top);
    buffer.push_back(left);
    buffer.push_back(bottom);
    buffer.push_back(top);
    buffer.push_back(right);
    buffer.push_back(bottom);
}

// Rhombus constructors
Rhombus::Rhombus(Vertex pos, float size, float speed, float duration, float frequency, SDL_Color Tint)
    : ParticleGroup(pos, size, speed, duration, Tint), period(frequency) {
    shadow_color = Tint;
    if (duration == -1)
        permanent = true;
    pos.z = pos.z - 1.f;
}

Rhombus::Rhombus(Gear* o, float size, float speed, float duration, float period, SDL_Color Tint)
    : ParticleGroup(o, size, speed, duration, Tint), period(period) {
    shadow_color = Tint;
    if (duration == -1)
        permanent = true;
    t.pos.z = t.pos.z - 1.f;
}

bool Rhombus::update(LayerTime &time) {
    // Check if we're done

    duration -= time.get_dt();
    deltat += time.get_dt();
    bool done = (duration <= 0) && !permanent;
    if (deltat > period && !done) {
        deltat -= period;
        if (horse != nullptr) {
            t.pos.z = horse->t.pos.z - 1.f;
            particles.emplace_back(horse->t.offset + horse->t.pos - Vertex(0,0,1), speed, strength);
        }
        else
            particles.emplace_back(Vertex(0,0,0), speed, strength);
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
    if (horse != nullptr)
        t.pos = horse->t.pos;
    for (auto& i : particles) {
        i.to_vertex(buffer, t.color);
        count+=6;
    }
    /*
    if (shadow())
        get_shadow_calc().apply_shadow(buffer, count);
        */
}