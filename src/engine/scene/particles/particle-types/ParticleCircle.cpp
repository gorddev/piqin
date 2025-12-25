#include "engine/scene/particles/particle-types/ParticleCircle.hpp"

using namespace geng;

PCircle::PCircle(FPos2D pos, int radius, float duration) : pos(pos), radius(radius), duration(duration) {
    direction.randomize();
    direction = direction.unit();
    initDuration = duration;
}

bool PCircle::update() {
    duration -= glb::scene.dt;
    if (duration <= 0)
        return true;
    return false;
}

// fancy circle calculating formula.
lni::vector<SDL_FRect> PCircle::to_rect(float &speed) {
    FPos2D tpos = direction * (speed * (initDuration - duration));
    int trad = static_cast<int>(radius * fabsf((duration)/initDuration));
    int d = 3 - (2*trad);
    int y = trad, x = 0;
    lni::vector<SDL_FRect> rects;
    while (x<=y) {
        rects.push_back({pos.x + tpos.x + x, pos.y +tpos.y + y, 1.f, 1.f});
        rects.push_back({pos.x + tpos.x - x, pos.y +tpos.y + y, 1.f, 1.f});
        rects.push_back({pos.x + tpos.x + x, pos.y +tpos.y - y, 1.f, 1.f});
        rects.push_back({pos.x + tpos.x - x, pos.y +tpos.y - y, 1.f, 1.f});
        rects.push_back({pos.x + tpos.x + y, pos.y +tpos.y + x, 1.f, 1.f});
        rects.push_back({pos.x + tpos.x - y, pos.y +tpos.y + x, 1.f, 1.f});
        rects.push_back({pos.x + tpos.x + y, pos.y +tpos.y - x, 1.f, 1.f});
        rects.push_back({pos.x + tpos.x - y, pos.y + tpos.y - x, 1.f, 1.f});
        x++;
        if (d < 0) {
            d= d + (4*x)+6;
        }
        else {
            y--;
            d = d +4 * (x-y)+10;
        }
    }
    return rects;
}

ParticleCircle::ParticleCircle(FPos2D pos, float size,
    float speed, float duration, float frequency,
    SDL_Color Tint)  : ParticleGroup(pos, size, speed, duration, Tint), period(frequency){
}
ParticleCircle::ParticleCircle(Object* o, float size,
    float speed, float duration, float frequency,
    SDL_Color Tint)  : ParticleGroup(o, size, speed, duration, Tint), period(frequency){
}

bool ParticleCircle::update() {
    // Check if we're done
    duration -= glb::scene.dt;
    deltat += glb::scene.dt;
    bool done = (duration <= 0) && !permanent;
    if (deltat > period && !done) {
        deltat -= period;
        if (horse != nullptr) {
            pos.z = horse->pos().z - 1.f;
            particles.push_back(PCircle(horse->offset() + horse->pos() - FPos2D(0,0,1), strength, 300));
        }
        else
            particles.push_back(PCircle(FPos2D(pos), strength, 300));
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

lni::vector<lni::vector<SDL_FRect> > ParticleCircle::to_vertex() {
    lni::vector<lni::vector<SDL_FRect>> rects;
    for (auto& i: particles) {
        rects.push_back(i.to_rect(speed));
    }
    return rects;
}
