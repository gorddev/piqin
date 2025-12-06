#pragma once
#include <SDL_rect.h>
#include <vector>
#include <iostream>

#include "../utilities/types/Vertex.hpp"
#include "../objects/Object.hpp"
#include "engine/textures/asset-info/RegistryConstants.hpp"

namespace gengine {
    // This is a white point on our texture
    inline SDL_FPoint white = {4.f/TEX_WIDTH, 4.f/TEX_HEIGHT};
    class ParticleGroup {
    protected:
        // Lets us have relevant overloads
        float duration;     // -1 means permanent.
        float strength = 1.0f;
        float speed = 1.0f;
        bool permanent = false;
        SDL_Color color = {255, 255, 255, 255};
        SDL_Color shadow_color = {0, 0, 0, 30};
        // Now here's where we buffer all the vertices
        std::vector<SDL_Vertex> buffer;

    public:
        // Id of the particlegroup
        int id = -1;
        // position of the group
        Vertex pos = Vertex(0,0,0);
        // If they're attatched to an object, they ride it like a horse
        Object* horse = nullptr;

        ParticleGroup(const Vertex pos, const float strength, const float speed, const float duration, const SDL_Color color) :
                pos(pos), buffer(20), duration(duration), strength(strength), speed(speed), color(color) { if (duration == -1) permanent = true; }
        ParticleGroup(Object* o, const float strength, const float speed, const float duration, const SDL_Color color)
            : strength(strength), buffer(20), speed(speed), color(color), pos(o->pos()), horse(o), duration(duration) { this->pos = o->pos(); this->pos.z = pos.z - 1.0f; if (duration == -1) permanent = true;}

        // Lets us update our particles. Should return true if done rendering.
        virtual bool update() = 0;  // pure virtual
        // Returns the render rectangles for our particle. MUST RETURN THE NUMBER OF VERTICES
        [[nodiscard]] virtual int to_vertex(std::vector<SDL_Vertex>& buffer) = 0;
        // Destructor
        virtual ~ParticleGroup() = default;
        // Gets the z index
        [[nodiscard]] float get_z() const { return pos.z;}
        // Gets the color
        SDL_Color& get_color() { return color; }
        SDL_Color& get_shadow_color() { return shadow_color; }
        // Ends a particlegroup's generation
        void end() { duration = 0; permanent = false; }
    };
}
