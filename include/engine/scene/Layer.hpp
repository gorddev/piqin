#pragma once
#include "layers/LayerContext.hpp"
#include "engine/rendering/DrawBatch.hpp"
#include "engine/input/InputHandler.hpp"
#include "engine/rendering/RenderBuffer.hpp"
#include "engine/scene/actors/ActorManager.hpp"
#include "engine/scene/morphs/MorphManager.hpp"
#include "engine/scene/font/FontList.hpp"
#include "engine/scene/initializer/Initializer.hpp"
#include "engine/scene/particles/ParticleManager.hpp"
#include "engine/scene/routes/RouteManager.hpp"
#include "engine/rendering/background/Background.hpp"
#include "engine/utilities/IDStack.hpp"
#include "events/EventManager.hpp"
#include "engine/scene/cells/CellBucket.hpp"

namespace geng {

    /** @brief The Layer is the primary target for adding objects, particles, ect. Each Layer operates independently of other layers.
     * @details Each layer by defualt is not z-indexed and is visible. To change this, access the relevant member
     * functions such as @code set_strict()@endcode or @code set_z_indexed()@endcode. Every layer has the following member funcctions
     */
    class Layer final {
    private:
        /// The TextureRegister of the Layer
        TextureRegister texreg;
    public:
        /// Allows the user to access the scene of the layer.
        LayerContext scene;
        /// Allows the user to initialize components of the Layer.
        Initializer init;
        /// Allows the user to access the input of the layer.
        InputHandler input;
        /// Allows users to directly add events.
        EventManager event;
        /// Allows users to directly add cells.[NOT RECOMMENDED FOR GAMEPLAY PURPOSES]
        CellBucket cell_bucket;
    private:
        /// Keeps track of the EngineContext
        EngineContext* world;
        /// The background of the Layer
        Background* background = nullptr;
        /// The ActorManager of the Layer
        ActorManager actor;
        /// The FontList of the layer
        FontList fl;
        /// The FrameManager of the layer
        FrameManager frame;
        /// The MorphManager of the Layer
        MorphManager morph;
        /// The RouteManager of the layer
        RouteManager route;
        /// The ParticleManager of the Layer
        ParticleManager particle;

        // ID-Assignment
        /// Keeps track of objects whose ids have been deleted.
        gutils::IDStack id_stack;
        /// Current topmost id.
        int top_id = 0;
        /// Determines whether to pop an id of the stack or assign a new one.
        int pop_id();

        // Adding/Removing Gears
        /// Holds all the renderable elements of the Layer
        std::vector<Gear*> gears;
        /// Adds one element to the Layer
        void add_gear(Gear* g);
        /// Removes the specifies element from the Layer
        void remove_gear(Gear* g);

    public:
        /// Constructor for a Layer
        // ReSharper disable once CppNonExplicitConvertingConstructor
        explicit Layer(std::string name);
        /// Destructor for a Layer
        ~Layer();

        /// Updates all elements in a scene
        void update(double dt);

        // <><><> Actors <><><>
        /// Adds an actor to the Layer
        void add_actor(Actor* a);
        /// Adds several actors to the scene.
        void add_actors(const std::vector<Actor*>& actors);
        /// Removes an actor from the engine.
        void remove_actor(const Actor *a);
        /// Removes a vector of actors from the engine.
        void remove_actors(const std::vector<Actor*>& objs);

        // <><><> Particle groups <><><>
        /// Attatches a particle to an actor and then adds it to the rendering pipline.
        void attach_particle(Actor* o, ParticleGroup* pg);
        /// Adds multiple particles to the rendering pipeline.
        void attach_particles(std::vector<ParticleGroup*>& pgs);
        /// Removes a particle
        void detach_particle(ParticleGroup* pg);
        /// Removes all particles attaches to an actor.
        void detach_particle(Actor *a);
        /// Detaches all particles attached to actors
        void detach_particles(const std::vector<Actor*>& pgs);
        /// Adds a detatched particle to the rendering pipeline
        void instantiate_particle(ParticleGroup *pg);

        // <><><> Morphs <><><>
        /// Adds a morph to any game object
        void apply_morph(Morph *e);
        /// Returns true if an object has a morph applied to it
        bool has_morph(Gear *g);
        /// Removes a specific morph
        void strip_morph(Morph* e);
        /// Removes all morphs from a game object
        void strip_morph(Gear *g);

        // <><><> Routes <><><>
        void set_route(Route* r);
        /// Sets a route TO a specific gear, with the provided offset. You use this if you target object is moving along a path.
        void set_route(Route *r, Gear*g, const Vertex &offset = {0.f, 0.f, 0.f});

        // <><><> Rendering styles <><><>
        /// Sets rendering order in order of objects added
        void set_render_strict();
        /// Renders everything based on z-index
        void set_render_z_index();
        /// Renders everything based on texture-id
        void set_render_batch();

        // <><><> Engine-Specific Functions <><><>
        /// Called by the engine to initialize the scene
        TextureRegister& _init();
        /// Called by the engine to render itself.
        std::vector<DrawBatch> _render_self(RenderBuffer& buffer);
        /// Orders the Layer based on ordering type. (batched, strict, z-indexed)
        void _order();
        /// Adds the engine context to the layer
        void _add_engine_context(EngineContext* ec);
        /// Finds a texture from the texture register
        SDL_Texture* _find_texture(int texture_id);
    };
}
