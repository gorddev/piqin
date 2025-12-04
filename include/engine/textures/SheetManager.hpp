#pragma once
#include <unordered_map>

#include "FrameSheet.hpp"
#include "Mesh.hpp"
#include "FrameState.hpp"
#include "engine/objects/Object.hpp"


namespace gengine {
    // Manages all the sprites and makes sure they update properly.
    class SheetManager {
    public:
        // Holds links from each sheet_id to a FrameSheet
        std::vector<textures::FrameSheet> sheets;

    public: // (again)
        // Constructor
        SheetManager();
        // Needed to render our textures
        void initialize(SDL_Renderer *renderer);
        // Adds proper frame info to an object
        void apply_framestate(Object& o);
        void apply_framestates(std::vector<Object*> objs);
        int get_texture_id(Mesh& m);
        // Allows us to easily access shit
        textures::FrameSheet& get_sheet(Object& o);

        // Updates the given framestates
        void update(std::vector<FrameState*> states);
    };
}
