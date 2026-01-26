#pragma once

#include "Collider.hpp"
#include "TileGrid.hpp"
#include "engine/mods/platformer/world/WorldManager.hpp"
#include "engine/layers/layer-subobjects/LayerCore.hpp"

namespace gan {

    /** The PhysicsMaestro basically handles everything physics in the engine, from updating colliders to determining collisions.
     * A lot of the PhysicsMaestro code is going to be hot-swappable so you don't need to use the engine defaults, and can customize according to your whims.*/
    class PhysicsMaestro {
    private:
        /// Contains the tilegrid for the next level
        TileGrid* grid = nullptr;
        /// Contains a reference to the game world
        WorldManager& world;
        /// Contains a reference to the LayerContext
        LayerCore& scene;
        /// Contains a vector of all the colliders
        std::vector<Collider*> colliders;
        /// Contains a vector of all the object's previous positions
        std::vector<vec2> prev_positions;
        /// Whether colliders can collide with each other
        bool internal_collisions = false;

        void collide(TileGrid &grid, int &id);
    public:
        /// Explicit constructor: must have access to the game world.
        explicit PhysicsMaestro(LayerCore& context, WorldManager& world);

        /// Adds a collider to the physics maestro
        void add_collider(Collider* collider);
        /// Removes a collider from the physic maestro
        void remove_collider(Collider* collider);

        /// Loads the current level's tilegrid from the gameworld
        void load_current_level();
        /// Performs collision checks on all the colliders
        void update();

        /// Returns a reference to the vector of colliders
        std::vector<Collider*>& get_colliders();

        /// Appends the hitboxes of everything to the render buffer
        void render_hitboxes(RenderBuffer &buffer, SDL_Color color, uint16_t thickness);

    };
}
