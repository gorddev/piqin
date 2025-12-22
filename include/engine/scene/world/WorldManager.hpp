#pragma once
#include "GameWorld.hpp"
#include "deserialization/WorldLoader.hpp"


namespace geng {

    class WorldManager {
    private:
        GameLevel* current_level = nullptr;
        GameWorld world;
        Tileset* tileset = nullptr;
        Camera& cam;
    public:
        explicit WorldManager(Camera& cam) : world(), cam(cam) {};

        void load_world(const std::string &filename, Tileset* t) {
            std::cerr << "loading world?\n";
            world = WorldLoader::read_world(filename);
            std::cerr << world.to_string() << std::endl;
            current_level = world.get_first_level();
            tileset = t;
        }

        void render_world(RenderBuffer& buffer) {
            if (world.magic) {
                TileBuffer tile_buffer(buffer, *tileset);
                world.render_world(tile_buffer, cam);
            }
        }

        int get_texture_id() {
            if (!world.magic)
                return -1;
            return tileset->get_texture_id();
        }

        GameLevel* get_current_level() {
            return current_level;
        }

        /// Returns the currently loaded game world.
        GameWorld& get_world() {
            return world;
        }

        /// Returns the tilesize of the current world.
        short get_tile_size() {
            if (!tileset) return -1;
            return tileset->tile_size();
        }
    };
}
