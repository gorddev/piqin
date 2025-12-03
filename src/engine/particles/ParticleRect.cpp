#include "engine/particles/ParticleRect.hpp"
#include "engine/gengine-globals/scene.hpp"
#include "engine/particles/ParticleRhombus.hpp"

using namespace gengine;

ParticleRect::ParticleRect(Vertex pos, float size, float speed, float duration, float period, short width, short height, SDL_Color Tint)
    : ParticleGroup(pos, size, speed, duration, Tint), period(period), width(width), height(height)  {}
ParticleRect::ParticleRect(Object* o, float size, float speed, float duration, float period, short width, short height, SDL_Color Tint)
    : ParticleGroup(o, size, speed, duration, Tint), period(period), width(width), height(height) {
    shadow_color = {0,0,0,0};
}

PRect::PRect(Vertex pos, float duration, short gerth, short width, short height)
    : pos(pos), duration(duration), gerth(gerth), width(width), height(height),initDuration(duration) { }

bool PRect::update() {
    duration -= glb::scene.dt;
    if (duration <= 0)
        return true;
    return false;
}

std::vector<SDL_FRect> PRect::to_rect(float& speed) const {
    std::vector<SDL_FRect> rects;
    float progress = (1.0f - duration/initDuration);
    int w = static_cast<int>(width + width * progress * speed*3.0f);
    int h = static_cast<int>(height + width * progress * speed*3.0f);
    int ger = static_cast<int>(gerth * (1-progress));

    rects.reserve(ger*4);
    float left= std::round(pos.x -w/2.0f);
    float top = std::round(pos.y - h/2.0f);

    float right  = left + std::round(w);
    float bottom = top + std::round(h);
    for (int i = 0; i < ger; i++){
        float insetL = left + i;
        float insetR = right- i- 1;
        float insetT = top +i;
        float insetB = bottom -i - 1;
        rects.push_back({ insetL, insetT, 1, insetB - insetT + 1 });
        rects.push_back({ insetR, insetT, 1, insetB - insetT + 1 });
        rects.push_back({ insetL, insetT, insetR - insetL + 1, 1 });
        rects.push_back({ insetL, insetB, insetR - insetL + 1, 1 });
    }
    return rects;
}

bool ParticleRect::update() {
    // Check if we're done
    duration -= glb::scene.dt;
    deltat += glb::scene.dt;
    bool done = (duration <= 0) && !permanent;
    if (horse != nullptr) {
        pos = horse->pos() + horse->offset();
        pos.z = horse->pos().z - 0.01f;
    }
    if (deltat > period && !done) {
        done = false;
        deltat -= period;
        if (horse != nullptr) {
            particles.emplace_back(pos, period, strength, width, height);

        }
        else
            particles.emplace_back(pos, period, strength, width, height);
    }
    for (auto it = particles.begin(); it != particles.end();) {
        if (it->duration <= 0)
            it = particles.erase(it);
        else {
            done = false;
            it->update();
            ++it;
        }
    }
    return done;
}

std::vector<std::vector<SDL_FRect>> ParticleRect::to_rect() {
    std::vector<std::vector<SDL_FRect>> rects;
    if (horse != nullptr)
        pos = horse->pos();
    for (auto& i : particles)
        rects.push_back(i.to_rect(speed));
    return rects;
}