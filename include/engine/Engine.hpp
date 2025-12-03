#pragma once
#include "render/RenderManager.hpp"
#include "particles/ParticleManager.hpp"
#include "objects/ObjectManager.hpp"
#include "input/InputManager.hpp"
#include "utilities/types/EngineElement.hpp"
#include "gengine-globals/Random.hpp"
#include "gengine-globals/Timer.hpp"


// The Engine makes sure everything gets to the right place
// It has the following tasks:
// * Routes objects to the objectManager
// * Routes particles to the particleManager
// * Routes UI elements to the UIManager
// * Sends render information to the renderManager (z-indexed)
namespace gengine {
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

        // Lets us assign IDs to each element
        int pop_id();
    public:
        // Constructor/destructor
        Engine();
        ~Engine();

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

        // Adding of things to other things
        void add(Object* o);
        void add(std::vector<Object*> objs, GENG_Sort sort = GENG_Sort::NONE);

        ParticleGroup *add(ParticleGroup *pg);
        void add(std::vector<ParticleGroup*>& pgs, GENG_Sort sort = GENG_Sort::NONE);

        // Allows us to remove objects
        void remove(const Object *o);
        void remove(const std::vector<Object*>& objs);
        void remove(ParticleGroup* pg);
        void remove(const std::vector<ParticleGroup*>& pgs);

        void remove(std::vector<int> ids);
    };
}