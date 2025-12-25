#include "engine/scene/particles/particle-types/Sparkle.hpp"

#include "engine/scene/sprites/Sprite.hpp"
#include "engine/layers/LayerState.hpp"

using namespace geng;
using namespace gfx;

SparkleInst::SparkleInst(const FPos2D &offset, float speed, float size) {
    pos = offset;
    velocity.randomize();
    velocity = velocity.unit();
    velocity *= speed;
    radius = size;
}

bool SparkleInst::update(double& dt) {
    duration -= dt;
    if (duration <= 0)
        return true;
    pos.x += velocity.x * dt* 0.05;
    pos.y += velocity.y * dt* 0.05;
    return false;
}

#define bpb(u, v) p2 = {u, v}; buffer.push_back(p2)
void SparkleInst::to_vertex(RenderBuffer& buffer, SDL_Color& color) {
    int rad = static_cast<int>(radius * duration / 1500.f);
    float x = roundf(pos.x);
    float y = roundf(pos.y);
    // Used vertices
    SDL_FPoint topleft = {x, y-rad-1.0f};
    SDL_FPoint bottomright = {x+1.0f, y+rad};
    // Assign our vertices
    // This is for the big top strip
    buffer.push_back(topleft);
    SDL_FPoint bleft = {x, y+rad+1.0f};
    buffer.push_back(bleft);
    buffer.push_back(bottomright);
    buffer.push_back(topleft);
    SDL_FPoint p2 = {x+1.0f, y-rad-1.0f};
    buffer.push_back(p2);
    buffer.push_back(bottomright);
    // This is for the two side strips
    // First side strip
    bpb(x-rad, y); bpb(x-rad, y-1.f); bpb(x,y-1.f);// bottom FPos2D
    bpb(x-rad, y); bpb(x,y); bpb(x,y-1.f); //top FPos2D
    // Second side strip
    bpb(x+1, y); bpb(x+1, y-1.f); bpb(x+1+rad, y-1.f); //bottom FPos2D
    bpb(x+1, y); bpb(x+1+rad, y); bpb(x+1+rad, y-1.f); // top FPos2D
}

// Sparkle constructors
Sparkle::Sparkle(FPos2D pos, float size, float speed, float duration, float frequency, SDL_Color Tint)
    : ParticleGroup(pos, size, speed, duration, {255, 255, 255, 255}), period(frequency) {
    shadow_color = Tint;
    if (duration == -1)
        permanent = true;
}

Sparkle::Sparkle(Gear* o, float size, float speed, float duration, float period, SDL_Color Tint)
    : ParticleGroup(o, size, speed, duration, {255, 255, 255, 255}), period(period) {
    shadow_color = Tint;
    if (duration == -1)
        permanent = true;
}

bool Sparkle::update(LayerState& time) {
    // Check if we're done
    duration -= time.get_dt();
    deltat += time.get_dt();
    bool done = (duration <= 0) && !permanent;
    if (!done) {
        while (deltat > period) {
            deltat -= period;
            if (payload != nullptr) {
                z_index = payload->z_index - 1;
                particles.emplace_back(payload->t.offset + payload->t.pos - FPos2D(0,0), speed, strength);
            }
            else
                particles.emplace_back(t.pos, speed, strength);
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

void Sparkle::to_vertex(RenderBuffer& buffer) {
    int count = 0;
    if (payload != nullptr)
        t.pos = payload->t.pos;
    for (auto& i : particles) {
        i.to_vertex(buffer, t.color);
        count += 18;
        buffer.push_shadow(18);
    }

}