#include "engine/particles/ParticleSparkle.hpp"

#include <iostream>

#include "engine/gengine-globals/scene.hpp"
#include "engine/objects/Object.hpp"


using namespace gengine;

Sparkle::Sparkle(const Vertex &offset, float speed, float size) {
    pos = offset;
    velocity.randomize();
    velocity = velocity.unit();
    velocity *= speed;
    radius = size;
}

bool Sparkle::update() {
    duration -= glb::scene.dt;
    if (duration <= 0)
        return true;
    pos.x += velocity.x * glb::scene.dt* 0.05;
    pos.y += velocity.y * glb::scene.dt* 0.05;
    return false;
}

#define bpb(u, v) buffer.push_back({{u, v}, color, white})
void Sparkle::to_vertex(std::vector<SDL_Vertex>& buffer, SDL_Color& color) {
    int rad = static_cast<int>(radius * duration / 1500.f);
    float x = roundf(pos.x);
    float y = roundf(pos.y);
    // Used vertices
    SDL_Vertex topleft = {{x, y-rad-1.0f},color, white };
    SDL_Vertex bottomright = {{x+1.0f, y+rad}, color, white};
    // Assign our vertices
    // This is for the big top strip
    buffer.push_back(topleft);
    buffer.push_back({{x, y+rad},color , white});
    buffer.push_back(bottomright);
    buffer.push_back(topleft);
    buffer.push_back({{x+1.0f, y-rad-1.0f}, color, white});
    buffer.push_back(bottomright);
    // This is for the two side strips
    // First side strip
    bpb(x-rad, y); bpb(x-rad, y-1.f); bpb(x,y-1.f);// bottom vertex
    bpb(x-rad, y); bpb(x,y); bpb(x,y-1.f); //top vertex
    // Second side strip
    bpb(x+1, y); bpb(x+1, y-1.f); bpb(x+1+rad, y-1.f); //bottom vertex
    bpb(x+1, y); bpb(x+1+rad, y); bpb(x+1+rad, y-1.f); // top vertex
}

// ParticleSparkle constructors
ParticleSparkle::ParticleSparkle(Vertex pos, float size, float speed, float duration, float frequency, SDL_Color Tint)
    : ParticleGroup(pos, size, speed, duration, {255, 255, 255, 255}), period(frequency) {
    shadow_color = Tint;
    if (duration == -1)
        permanent = true;
}

ParticleSparkle::ParticleSparkle(Object* o, float size, float speed, float duration, float period, SDL_Color Tint)
    : ParticleGroup(o, size, speed, duration, {255, 255, 255, 255}), period(period) {
    shadow_color = Tint;
    if (duration == -1)
        permanent = true;
}

bool ParticleSparkle::update() {
    // Check if we're done
    duration -= glb::scene.dt;
    deltat += glb::scene.dt;
    bool done = (duration <= 0) && !permanent;
    if (!done) {
        while (deltat > period) {
            deltat -= period;
            if (horse != nullptr) {
                pos.z = horse->pos().z - 0.01f;
                particles.push_back(Sparkle(horse->offset() + horse->pos() - Vertex(0,0,0.4), speed, strength));
            }
            else
                particles.push_back(Sparkle(Vertex(0,0,0), speed, strength));
        }
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

int ParticleSparkle::to_vertex(std::vector<SDL_Vertex>& buffer) {
    int count = 0;
    if (horse != nullptr)
        pos = horse->pos();
    for (auto& i : particles) {
        i.to_vertex(buffer, color);
        count += 18;
    }
    return count;
}