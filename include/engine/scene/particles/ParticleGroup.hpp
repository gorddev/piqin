#pragma once
#include <SDL.h>
#include <vector>
#include <iostream>

#include "../../types/positioning/FPos2D.hpp"
#include "engine/scene/sprites/Sprite.hpp"

namespace geng {
    /**
     * @brief ParticleGroup is an abstract base class which defines base behavior for particle effects.
     * @details If you want particles to display, they must inherit from ParticleGroup and be added
     * to the engine via @code engine.add_particle(...) @endcode.
     * ParticleGroups have several key member variables:
     * 1. float duration -> duration of the particle effect (-1 implies permanent)
     * 2. float strength -> determines the size/ferocity of the particles.
     * 3. float speed -> speed of the particles
     * 4. bool permanent -> if the ParticleGroup is permanent (auto-assigned by constructor)
     * 5. SDL_Color color -> color of the particle effect
     * 6. SDL_COlor shadow_color -> color of the shadow of the particle effect
     * 7. gengine::FPos2D pos -> location of the ParticleGroup
     * 8. int id -> used for indexing purposes by the engine
     * 9. Transform2D* horse -> if the ParticleGroup is attatched to a transform object, horse is not nullptr.
     * @warning There exists two essential functions to override for ParticleGroups:
     * @code update()@endcode and @code to_vertex()@endcode. @code update()@endcode returns true when the particle effect is over. @code to_vertex(std::vector<SDL_Vertex>&buffer)@endcode
     * takes in the FPos2D buffer DIRECTLY FROM THE RENDERER. Thus, you must manually specify the proper vertices for your particle effect manually. There is no nice tool to do this for you.
     * If you do not properly add your @code SDL_Vertices@endcode in sets of three to the render buffer, visuals likely will bug and not work, and there is a high probability of memory corruption.
     * @note There exists an "inline SDL_FPoint white" in this definition. Just make sure you set the "4.f and 4.f" to a point on your atlas texture that is RGB(255,255,255,255)
    */
    class ParticleGroup : public Gear {
    protected:
        // Lets us have relevant overloads
        float duration;     // -1 means permanent.
        float strength = 1.0f;
        float speed = 1.0f;
        bool permanent = false;
        SDL_Color shadow_color = {0, 0, 0, 30};

    public:
        // If they're attatched to an object, they ride it like a horse
        Gear* horse = nullptr;

        ParticleGroup(const FPos2D pos, const float strength, const float speed, const float duration, const SDL_Color color) :
                Gear(pos), duration(duration), strength(strength), speed(speed) { if (duration == -1) permanent = true; t.color = color; }
        ParticleGroup(Gear* g, const float strength, const float speed, const float duration, const SDL_Color color)
            : strength(strength), speed(speed), Gear(g->t.pos), horse(g), duration(duration) { z_index = g->z_index - 1; t.color = color; if (duration == -1) permanent = true;}

        // Lets us update our particles. Should return true if done rendering.
        virtual bool update(LayerState& t) = 0;  // pure virtual
        // Returns the rendering buffer for our particle. MUST RETURN THE NUMBER OF VERTICES
        void to_vertex(RenderBuffer& buffer) override = 0;
        // Destructor
        ~ParticleGroup() override = default;
        // Gets the color
        SDL_Color& get_color() { return t.color; }
        SDL_Color& get_shadow_color() { return shadow_color; }
        // Ends a particlegroup's generation
        void end() { duration = 0; permanent = false; }
    };
}
