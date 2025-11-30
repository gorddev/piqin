#pragma once
#include <SDL_rect.h>
#include <vector>

#include "../utilities/types/Vertex.hpp"

namespace gengine {
    class ParticleGroup {
    protected:
        // Lets us have relevant overloads
        float duration = -1.0f;
        float strength = 1.0f;
        float speed = 1.0f;
        SDL_Color color = {255, 255, 255, 255};
    public:

        int id = -1;
        Vertex pos = Vertex(0,0,0);

        ParticleGroup(const Vertex &pos, float z_index, const float strength, const float speed, const float duration, const SDL_Color color) :
            duration(duration), strength(strength), speed(speed), color(color), pos(pos) {}

        // Lets us update our particles
        virtual bool update() = 0;  // pure virtual
        // Returns the render rectangles for our particle
        virtual std::vector<std::vector<SDL_FRect>> get_particles() = 0;
        // Destructor
        virtual ~ParticleGroup() = default;
        // Gets the z index
        [[nodiscard]] float get_z() const { return pos.z;}
        // Gets the color
        SDL_Color& get_color() { return color; }
    };
}