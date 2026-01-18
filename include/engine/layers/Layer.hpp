#pragma once
#include "LayerContext.hpp"
#include "LayerInit.hpp"
#include "engine/rendering/DrawBatch.hpp"
#include "engine/input/InputHandler.hpp"
#include "engine/rendering/RenderBuffer.hpp"
#include "engine/scene/sprites/SpriteManager.hpp"
#include "engine/scene/morphs/MorphManager.hpp"
#include "engine/scene/font/FontList.hpp"
#include "engine/scene/initializer/Initializer.hpp"
#include "engine/scene/particles/ParticleManager.hpp"
#include "engine/scene/routes/RouteManager.hpp"
#include "engine/rendering/background/Background.hpp"
#include "engine/scene/banners/BannerManager.hpp"
#include "engine/scene/cells/CellBucket.hpp"
#include "engine/scene/events/EventManager.hpp"
#include "engine/scene/physics/PhysicsMaestro.hpp"
#include "engine/scene/tilesets/TileList.hpp"
#include "engine/scene/world/WorldManager.hpp"

namespace gan {

    /** @brief The Layer is the primary target for adding objects, particles, ect. Each Layer operates independently of other layers.
     * @details Each layer by defualt is not z-indexed and is visible. To change this, access the relevant member
     * functions such as @code set_strict()@endcode or @code set_z_indexed()@endcode. Every layer has the following member funcctions
     */
    class Layer {
    private:
        /// Name of the layer.
        const gan::fstring<10> name;
        /// Also keeps track of the core of the engine, like camera and scene width/height.
        EngineContext& core;
    public:
        /// Keeps track of the camera
        Camera camera;
        /// The scene contains important information concerning the layer.
        /// Most of the member variables below recieve a reference to this scene object.
        LayerContext scene;
    private:
        /// Contains the engine's texture register.
        TextureRegister& texreg;
    public:
        /// Allows the user to access the input of the layer.
        InputHandler input;
        /// Allows users to directly add events.
        EventManager events;
        /// Allows users to directly add cells.[NOT RECOMMENDED FOR GAMEPLAY PURPOSES]
        CellBucket cell_bucket;
    private:
        // <><><> Three primary engine types <><><>
        /// The SpriteManager of the Layer
        SpriteManager sprites;
        /// The BannerManager of the layer
        BannerManager banners;
        /// The ParticleManager of the Layer
        ParticleManager particles;

        // <><><> Texture Manipulation/Retrieval
        /// The background of the Layer
        Background* background = nullptr;
        /// The FrameList of the layer
        FrameList frames;
        /// The TileList of the Layer
        TileList tiles;

        // <><><> Effects applied to engine tyeps <><><>
        /// The MorphManager of the Layer
        MorphManager morphs;
        /// The RouteManager of the layer
        RouteManager routes;

        // <><><> Physics/Worldbuilding <><><>
        /// The WorldManager of the Layer
        WorldManager world;
        /// The Physics Manager of the Lyaer
        PhysicsMaestro physics;
    public:
        // <><><> Initialization <><><>
        /// Allows the user to very easily call for initialization.
        Initializer init;
    private:
        // Adding/Removing Gears
        /// Holds all the renderable elements of the Layer
        gch::vector<Gear*> gears;
        /// Used for id-assignmnet
        int id = 0;
        /// Adds one element to the Layer
        bool add_gear(Gear* g);
        /// Removes the specifies element from the Layer
        void remove_gear(Gear* g);
    public:
        /// Constructor for a Layer
        explicit Layer(LayerInit& layer_init, gan::fstring<10> name);
        /// Destructor for a Layer
        virtual ~Layer();

        /// Updates all elements in a scene. Override if you know what you're doing.
        virtual void update(double dt);

        // <><><> Sprites <><><>
        /// Adds an sprite to the Layer
        void add_sprite(Sprite* a);
        /// Adds several sprites to the scene.
        void add_sprites(gch::vector<Sprite *> &new_sprites);
        /// Removes an sprite from the engine.
        void remove_sprite(Sprite *a);
        /// Removes a vector of sprites from the engine.
        void remove_sprites(gch::vector<Sprite *> &objs);

        // <><><> Banners <><><>
        /// Adds a banner to the layer
        void add_banner(Banner* b);
        /// Adds several banners to the layer
        void add_banners(gch::vector<Banner *> &banners);
        /// Removes a banner from the scene
        void remove_banner(Banner* b);
        /// Removes several banners from the scene
        void remove_banners(gch::vector<Banner *> &banners);

        // <><><> Fonts & Frames <><><>
        /// Gets a frametable
        FrameTable& get_frame_table(int index);

        FrameTable &ftab(int index);

        // <><><> Particle groups <><><>
        /// Attatches a particle to an sprite and then adds it to the rendering pipline.
        void attach_particle(Gear *g, ParticleGroup *pg);
        /// Adds multiple particles to the rendering pipeline.
        void attach_particles(gch::vector<ParticleGroup*>& pgs);
        /// Removes a particle
        void detach_particle(ParticleGroup* pg);
        /// Removes all particles attaches to an sprite.
        void detach_particle(Sprite *a);
        /// Detaches all particles attached to sprites
        void detach_particles(gch::vector<Sprite *> &pg);
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

        // <><><> Physics <><><>
        /// Adds a collider to the physics engine
        void link_collider(Collider* c);
        /// Removes a collider from the physics engine
        void unlink_collider(Collider* c);

        // <><><> Routes <><><>
        void set_route(Route* r);
        /// Sets a route TO a specific gear, with the provided offset. You use this if you target object is moving along a path.
        void set_route(Route *r, Gear*g, const FPos2D &offset = {0,0});

        // <><><> Rendering styles <><><>
        /// Sets rendering order in order of objects added
        void set_render_strict();
        /// Renders everything based on z-index
        void set_render_z_index();
        /// Renders everything based on texture-id
        void set_render_batch();

        // <><><> Worlds <><><>
        /// Sets the current world given the filename
        void load_world(gan::hstring filename, int tileset_num);

        // <><><> Tilesets <><><>
        /// Lets you get a tileset from the tileset list
        Tileset& get_tileset(int index);

        // <><><> Returns the center of the scene <><><>
        FPos2D get_scene_center() const;

        gutils::SparseVector<ParticleGroup> &
        active_particles();

        // <><><> Returns whether we're active or not.
        bool is_active() const;
        // <><><> Returns the name of the layer
        gan::fstring<10> get_name() const;
        /// Sets the speed of the layer
        void set_speed(float speed);
    protected:
        friend class Engine;
        friend class Renderer;
        // <><><> Engine-Specific Functions <><><>
        /// Called by the engine to render itself. Only override if you really trust yourself.
        virtual void render_self(RenderBuffer &buffer);
    private:
        /// Renders the gears in the layer
        void render_gears(RenderBuffer &buffer);
        /// Renders the debug grid in the layer
        void render_grid(RenderBuffer &buffer, int grid_div, int thickness, bool subdiv = false) const;
        /// Called by render_self to optionally render hitboxes. Only override if you really know what you're doing.
        virtual void render_hitboxes(RenderBuffer &buffer);

        /// Orders the Layer based on ordering type. (batched, strict, z-indexed)
        void order();
        /// Finds a texture from the texture register
        SDL_Texture* _find_texture(int texture_id);

    };
}
