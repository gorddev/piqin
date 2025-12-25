#pragma once
#include "GameWorld.hpp"
#include "deserialization/WorldLoader.hpp"

#include "engine/debug/logging/LogSource.hpp"


namespace geng {

    class WorldManager {
    private:
        GameLevel* current_level = nullptr;
        GameWorld world;
        Tileset* tileset = nullptr;
        const Camera& cam;
    public:
        explicit WorldManager(const Camera& cam) : world(), cam(cam) {
            glog::note << "WorldManager for random ass layer formed." << glog::endlog;
        };

        void load_world(hstring filename, Tileset* t) {
            glog::dev << "loading world?\n";
            world = WorldLoader::read_world(filename);
            fstring<900> buffer;
            auto view = buffer.wrap();
            world.to_fstring(view);
            glog::dev << buffer.cstr() << glog::endlog;
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
