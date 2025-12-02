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

std::vector<SDL_FRect> Rhombus::to_rect() const {
    std::vector<SDL_FRect> rects;
    float rad = radius * duration / 1500;
    rects.reserve(rad*rad*2);
    // first get the width at each y point
    for (short y = -rad; y <= rad; y++) {
        const float width = rad - abs(y);
        const float pixely =  pos.y + y;
        // fill out the width here
        for (short x = -width; x <= width; x++) {
            rects.push_back({ pos.x + x, pixely, 1, 1});
        }
    }
    // return our hard work
    return rects;
}

// ParticleRhombus constructors
ParticleRhombus::ParticleRhombus(Vertex pos, float size, float speed, float duration, float frequency, SDL_Color Tint)
    : ParticleGroup(pos, size, speed, duration, {255, 255, 255, 255}), period(frequency) {
    shadow_color = Tint;
    if (duration == -1)
        permanent = true;
}

ParticleRhombus::ParticleRhombus(Object* o, float size, float speed, float duration, float period, SDL_Color Tint)
    : ParticleGroup(o, size, speed, duration, {255, 255, 255, 255}), period(period) {
    shadow_color = Tint;
    if (duration == -1)
        permanent = true;
}

bool ParticleRhombus::update() {
    // Check if we're done

    duration -= glb::scene.dt;
    deltat += glb::scene.dt;
    bool done = (duration > 0);
    if (deltat > period && (permanent || duration > 0)) {

        done = false;
        deltat -= period;
        if (horse != nullptr) {
            particles.push_back(Rhombus(horse->offset() + horse->pos() - Vertex(0,0,0.1), speed, strength));
            pos.z = horse->pos().z - 0.01f;
        }
        else
            particles.push_back(Rhombus(Vertex(0,0,0), speed, strength));
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

std::vector<std::vector<SDL_FRect>> ParticleRhombus::to_rect() {
    std::vector<std::vector<SDL_FRect>> rects;
    if (horse != nullptr)
        pos = horse->pos();
    for (auto& i : particles)
        rects.push_back(i.to_rect());
    return rects;
}