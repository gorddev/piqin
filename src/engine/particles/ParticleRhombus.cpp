#include "engine/particles/ParticleRhombus.hpp"

#include <iostream>

#include "engine/gengine-globals/scene.hpp"
#include "engine/objects/Object.hpp"


using namespace gengine;

Rhombus::Rhombus(const Vertex &offset, float speed, float size) {
    pos = offset;
    velocity.randomize();
    velocity = velocity.unit();
    velocity *= speed;
    radius = size;
}

bool Rhombus::update() {
    duration -= glb::scene.dt;
    if (duration <= 0)
        return true;
    pos.x += velocity.x * glb::scene.dt* 0.05;
    pos.y += velocity.y * glb::scene.dt* 0.05;
    return false;
}

void Rhombus::to_vertex(std::vector<SDL_Vertex>& buffer, SDL_Color& color) const {
    // Radius
    int rad = static_cast<int>(radius * duration / 1500.f);
    // Vertex locations
    SDL_Vertex top = {{pos.x, pos.y - rad}, color, white};
    SDL_Vertex bottom = {{pos.x, pos.y + rad},  color, white};
    SDL_Vertex left = {{pos.x - rad, pos.y}, color, white, };
    SDL_Vertex right = {{pos.x + rad, pos.y}, color, white};
    // Send to buffer
    buffer.push_back(top);
    buffer.push_back(left);
    buffer.push_back(bottom);
    buffer.push_back(top);
    buffer.push_back(right);
    buffer.push_back(bottom);
}

// ParticleRhombus constructors
ParticleRhombus::ParticleRhombus(Vertex pos, float size, float speed, float duration, float frequency, SDL_Color Tint)
    : ParticleGroup(pos, size, speed, duration, {255, 255, 255, 255}), period(frequency) {
    shadow_color = Tint;
    if (duration == -1)
        permanent = true;
    pos.z = pos.z - 1.f;
}

ParticleRhombus::ParticleRhombus(Object* o, float size, float speed, float duration, float period, SDL_Color Tint)
    : ParticleGroup(o, size, speed, duration, {255, 255, 255, 255}), period(period) {
    shadow_color = Tint;
    if (duration == -1)
        permanent = true;
    pos.z = pos.z - 1.f;
}

bool ParticleRhombus::update() {
    // Check if we're done

    duration -= glb::scene.dt;
    deltat += glb::scene.dt;
    bool done = (duration <= 0) && !permanent;
    if (deltat > period && !done) {
        deltat -= period;
        if (horse != nullptr) {
            pos.z = horse->pos.z - 1.f;
            particles.emplace_back(horse->offset + horse->pos - Vertex(0,0,1), speed, strength);
        }
        else
            particles.emplace_back(Vertex(0,0,0), speed, strength);
    }
    for (auto it = particles.begin(); it != particles.end();) {
        if (it->duration <= 0) {
            it = particles.erase(it);
        }
        else {
            done = false;
            it->update();
            ++it;
        }
    }
    return done;
}

int ParticleRhombus::to_vertex(std::vector<SDL_Vertex>& buffer) {
    int count = 0;
    if (horse != nullptr)
        pos = horse->pos;
    for (auto& i : particles) {
        i.to_vertex(buffer, color);
        count+=6;
    }
    return count;
}