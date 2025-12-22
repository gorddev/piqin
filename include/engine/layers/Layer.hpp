#pragma once
#include "LayerContext.hpp"
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
#include "engine/utilities/IDStack.hpp"
#include "engine/scene/banners/BannerManager.hpp"
#include "engine/scene/cells/CellBucket.hpp"
#include "engine/scene/events/EventManager.hpp"
#include "engine/scene/physics/PhysicsMaestro.hpp"
#include "engine/scene/tilesets/TileList.hpp"
#include "engine/scene/world/WorldManager.hpp"

namespace geng {

    /** @brief The Layer is the primary target for adding objects, particles, ect. Each Layer operates independently of other layers.
     * @details Each layer by defualt is not z-indexed and is visible. To change this, access the relevant member
     * functions such as @code set_strict()@endcode or @code set_z_indexed()@endcode. Every layer has the following member funcctions
     */
    class Layer {
    public:
        /// Allows the user to access the scene of the layer.
        LayerContext scene;
    private:
        /// The TextureRegister of the Layer
        TextureRegister texreg;
    public:
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
        EngineContext* engine_context;
        /// The background of the Layer
        Background* background = nullptr;
        /// The SpriteManager of the Layer
        SpriteManager sprite;
        /// The BannerManager of the layer
        BannerManager banner;
        /// The FontList of the layer
        FontList fl;
        /// The FrameList of the layer
        FrameList frame;
        /// The MorphManager of the Layer
        MorphManager morph;
        /// The RouteManager of the layer
        RouteManager route;
        /// The ParticleManager of the Layer
        ParticleManager particle;
        /// The TileList of the Layer
        TileList tilesets;
        /// The WorldManager of the Layer
        WorldManager world;
        /// The Physics Manager of the Lyaer
        PhysicsMaestro physics;

        // ID-Assignment
        /// Keeps track of the layer's id
        int id = -1;
        /// Keeps track of whether the layer is active or not
        bool active = false;

        // Adding/Removing Gears
        /// Holds all the renderable elements of the Layer
        std::vector<Gear*> gears;
        /// Adds one element to the Layer
        bool add_gear(Gear* g);
        /// Removes the specifies element from the Layer
        void remove_gear(Gear* g);
    public:
        /// Constructor for a Layer
        explicit Layer(std::string name);
        /// Destructor for a Layer
        virtual ~Layer();

        /// Updates all elements in a scene. Override if you know what you're doing.
        virtual void update(double dt);

        // <><><> Sprites <><><>
        /// Adds an sprite to the Layer
        void add_sprite(Sprite* a);
        /// Adds several sprites to the scene.
        void add_sprites(const std::vector<Sprite*>& sprites);
        /// Removes an sprite from the engine.
        void remove_sprite(Sprite *a);
        /// Removes a vector of sprites from the engine.
        void remove_sprites(const std::vector<Sprite*>& objs);

        // <><><> Banners <><><>
        /// Adds a banner to the layer
        void add_banner(Banner* b);
        /// Adds several banners to the layer
        void add_banners(const std::vector<Banner*>& banners);
        /// Removes a banner from the scene
        void remove_banner(Banner* b);
        /// Removes several banners from the scene
        void remove_banners(const std::vector<Banner*>& banners);

        // <><><> Fonts & Frames <><><>
        /// Gets a font
        Font& get_font(int index);
        /// Gets a frametable
        FrameTable& get_frame_table(int index);

        // <><><> Particle groups <><><>
        /// Attatches a particle to an sprite and then adds it to the rendering pipline.
        void attach_particle(Gear *g, ParticleGroup *pg);
        /// Adds multiple particles to the rendering pipeline.
        void attach_particles(std::vector<ParticleGroup*>& pgs);
        /// Removes a particle
        void detach_particle(ParticleGroup* pg);
        /// Removes all particles attaches to an sprite.
        void detach_particle(Sprite *a);
        /// Detaches all particles attached to sprites
        void detach_particles(const std::vector<Sprite*>& pgs);
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
        /// Removes a collider from the physic engine
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
        void load_world(std::string filename, int tileset_num);

        // <><><> Tilesets <><><>
        /// Lets you get a tileset from the tileset list
        Tileset& get_tileset(int index);

        // <><><> Returns the center of the scene <><><>
        FPos2D get_scene_center() const;

        // <><><> Engine-Specific Functions <><><>
        /// Called by the engine to initialize the scene. Only override if you really trust yourself.
        virtual TextureRegister& _init();
        /// Called by the engine to render itself. Only override if you really trust yourself.
        virtual std::vector<DrawBatch> _render_self(RenderBuffer& buffer);
        /// Renders the gears in the layer
        void _render_gears(RenderBuffer &buffer,DrawBatch &current_batch,std::vector<DrawBatch> &batches);
        /// Renders the debug grid in the layer
        void _render_grid(RenderBuffer &buffer, int grid_div, int thickness, bool subdiv = false) const;
        /// Called by _render_self to optionally render hitboxes. Only override if you really know what you're doing.
        virtual void _render_hitboxes(RenderBuffer &buffer);

        /// Orders the Layer based on ordering type. (batched, strict, z-indexed)
        void _order();
        /// Adds the engine context to the layer
        void _add_engine_context(EngineContext* ec);
        /// Finds a texture from the texture register
        SDL_Texture* _find_texture(int texture_id);
        /// Sets the layer's active status
        void _set_active(bool status);

        bool is_active() const;
    };
}
