#pragma once
#include "effects/EffectManager.hpp"
#include "render/RenderManager.hpp"
#include "particles/ParticleManager.hpp"
#include "objects/ObjectManager.hpp"
#include "input/InputManager.hpp"
#include "utilities/types/EngineElement.hpp"
#include "gengine-globals/Random.hpp"
#include "gengine-globals/Timer.hpp"
#include "textures/SheetManager.hpp"


namespace gengine {
    /**
     * @brief The engine handles input, effect, particles, ECS, rendering, sound, and text. You do not have direct control over how the engine does its job. You only feed it inputs, and it gives you an output, and sends you keyboard input.
     * @details In order to use the engine, you have to speak in its language: gengine::Object, gengine::Effect, gengine::ParticleGroup, gengine::inputTarget, gengine::Font, and gengine::Sound. It is recommended to review these files before continuing with the engine.
     * Using the engine's features can be a bit of a hurdle to get used to, but it is designed to be somewhat intuitive.
     * - To add objects you use @code engine.add_object(...) @endcode. The engine will then display this object on screen in accordance with its gengine::Transform and gengine::FrameState.
     * - To add particles, you use @code engine.add_particle(...) @endcode. You have the option to attach the particle to a gengine::Object, but it is not required.
     * - To add effects, you use @code engine.add_effect(...) @endcode. You must add the effect to a gegine::Transform or gengine::Object. Effects are dependent on preexisting objects.
     */
    class Engine {
    private:
        // Keeps track of all of our render targets
        std::vector<EngineElement> elements;
        // Lets us keep track of unused IDs
        utils::IDStack id_stack;
        int top_id = 0;
        // Keeps track of all the engine shit
        Camera cam;
        SheetManager sm;
        ObjectManager om;
        InputManager input;
        RenderManager rm;
        ParticleManager pm;
        EffectManager em;

        // Lets us assign IDs to each element
        int pop_id();
    public:
        // Constructor/destructor
        Engine();
        ~Engine() = default;

        // Initialize the engine (must be done in main)
        void initialize();

        // Runs the engine
        bool tick(double time);

        // Sends things to the renderer & presents in one step
        void render();

        // Sets an input target
        void set_input_target(InputTarget* t);

        // Application of particles to objects.
        void attach_new_particle(Object *o, ParticleGroup *pg);
        void remove_attached_particle(Object *o);

        /* Additions to the engine!!! */
        // Objects
        void add_object(Object* o);
        void add_objects(std::vector<Object*> objs);
        // Particle Groups
        void add_particle(ParticleGroup *pg);
        void add_particle(Object* o, ParticleGroup* pg);        // Adds a particle group to an object.
        void add_particles(std::vector<ParticleGroup*>& pgs);
        // Effects
        void add_effect(Object& o, Effect* e);
        void add_effect(Transform& t, Effect* e);

        // Allows us to remove objects
        // Objects
        void remove_object(const Object *o);
        void remove_objects(const std::vector<Object*>& objs);
        // Particles
        void remove_particle(ParticleGroup* pg);
        void remove_particles(const std::vector<ParticleGroup*>& pgs);
        // Effects
        void remove_effect(const Object* o);

        void remove(std::vector<int> ids);
    };
}