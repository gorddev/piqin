#pragma once

#include "Frame.hpp"
#include "FrameState.hpp"

namespace gengine::textures {
    // Frame sheets hold all the frame data for a specific type of sprite
    // It's essentially just a lookup table for "what comes next in this animation?")
    class FrameSheet {
    private:
        // Holds all the different frame possibilities, indexed by state
        std::vector<std::vector<Frame>> frames;
        // Holds the index to their texture
        int texture_id;

    public:
        FrameSheet();
        // Creates a frameSheet from a manual vector of frames
        FrameSheet(std::string path, int texWidth, int texHeight,
            std::vector<std::vector<Quad>> quads);
        // Creates a templated frameSheet from preset values
        FrameSheet(std::string path, int texWidth, int texHeight,
            int startx, int starty, int w, int h,
            short numColumns, short numFrames);
        // Creates a templated frameSheet with overrides
        FrameSheet(std::string path, int texWidth, int texHeight,
            int startx, int starty,
            int w, int h, short numColumns, short numFrames,
            // For overrides. int -> number of the frame you want to start overriding at
            // std::vector<Frames> -> frames you want to override
            // Override will continue with the template after overriding
            //      @ the override point + vector<Frame>.size()
            std::vector<std::pair<int, std::vector<Quad>>> overrides);

        // Returns the texture id associate with this frame
        int get_texture_id();

        Frame& get_frame(int animationNum, int frameNum = 0);

        // Lets us move a framestate forward
        void update_frame(FrameState& s);
    };
}
