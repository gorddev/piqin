#pragma once

#include <iostream>

#include "Frame.hpp"
#include "AnimInfo.hpp"
#include "../../types/Quad.hpp"
#include "engine/utilities/image-info/IMGDecoder.hpp"


namespace geng {
    /**
     * FrameTables hold a double-vector of @code gengine::Frame@endcode objects. Their intended use is to serve as a data bank to keep track of animation tables for @code AnimInfo@endcode objects.
     * @details FrameTables are controlled by the @code gengine::FrameManager@endcode in the engine.
     * - It is recommended to review the structure of a @code gengine::Quad@endcode before making a FrameTable.
     * - When the engine detects a frame change, it sends the changed AnimInfo to it's associated FrameTable, and calls this FrameTable's @code FrameTable::update_frame(AnimInfo&)@endcode function.
     * - When the engine initializes objects into the pipeline, it calls @code FrameTable::get_frame()@endcode on them.
     * @warning It is up to the user to decide how they will keep track of animation numbers and frame numbers. There exists no easy tool to do this. If you are not worried about low-level memory optimization,
     * I highly recommend making an @code unordered_map<string, int>@endcode linking readable string names to animation numbers. Macros are also a good tool if you are worried about memory and don't care about the global namespace pollution.
     */
    class FrameTable final {
    private:
        /// Holds all the frame-information, indexed by animation number, and then frame number -> frames[anim_num][frame_num].
        std::vector<std::vector<Frame>> frames;
        /// Holds temporary vector of quads that will be deleted
        std::vector<std::vector<Quad>> quads;
        /// Links this FrameSheet to a texture in the @code TextureRegistry @endcode
        int texture_id = -1;

    public:
        /// For this constructor, you manually specify the x, y, width, and height of each Frame with a Quad {x, y, w, h}. Use an initializer list.
        FrameTable(std::vector<std::vector<Quad>> quads);
        /// For this constructor, you specify a start x, y, width, height, numColumns, and numAnimations. Then, this constructor will go horizontaly, move onto the next row, go horizontally, ect. to create frames with the specified width and height.
        FrameTable(int startx, int starty, int w, int h, short numColumns, short numAnimations);
        /// For this constructor, you specify a start x, y, width, height, numColumns, and numAnimations. Then, this constructor will go horizontaly, move onto the next row, go horizontally, ect. to create frames with the specified width and height. You can
        /// specify a number of overrides with a @code std::vector<std::pair<int, std::vector<Quad>>>@endcode. Here, the int represent the animation_index you want to start overriding at, and the @code std::vector<Quad> @endcode contains your specified quad
        /// overrides. Each override increments numAnimations by one, so make sure for each override, you consider it in your numAnimations varible. Overrides also do NOT alter the x, y, w, h parameters, so the function will continue as if the override didn't exist.
        /// Please consider this when forming a spritesheet.
        FrameTable(int startx, int starty, int w, int h, short numColumns, short numAnimations,
                const std::vector<std::pair<int, std::vector<Quad>>>& overrides);

        /// Initializes the frame table with the texture it will use.
        void _init(IMG_Info& info);

        /// Returns the given frame at the animationNum. frameNum defaults to 0.
        Frame& get_frame(int animationNum, int frameNum = 0);

        /// Updates an animator object's frame.
        void update_frame(AnimInfo& s);

        /// Returns the texture id associated with this FrameTable.
        [[nodiscard]] int get_texture_id() const;
        void set_texture_id(int id);
    };
}

