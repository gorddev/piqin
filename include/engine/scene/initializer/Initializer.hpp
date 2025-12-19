#pragma once
#include <vector>
#include <string>

#include "TextureRegister.hpp"
#include "engine/scene/animation//FrameList.hpp"
#include "engine/scene/animation/FrameTable.hpp"
#include "engine/scene/font/Font.hpp"
#include "engine/scene/font/FontList.hpp"

namespace geng {
    /**
     * The initializer takes care of making sure object definitions get to the right place, and that textures are initialized
     */
    class Initializer final {
        /// Contains all the frameTables that will be created
        std::vector<FrameTable> frameTables;
        int table_num = 0;
        /// Contains all the textures that will need to be created
        TextureRegister& tex_reg;
        /// Contains all the fonts the user specifies
        std::vector<Font> fonts;
        int font_num = 0;

        /// Clears out the system font
        void _clear();

        /// The layerContext object
        LayerContext& scene;
    public:
        explicit Initializer(TextureRegister& tex_reg, LayerContext& scene);

        /// Texture
        int texture(const std::string &path, bool _internal = false);
        /// Sets up a FrameTable and its corresponding texture to be initialized. Returns the ID of the frameTable
        int frame_table(const std::string &path, FrameTable ft);
        /// Sets up a Font and its corresponding texture to be initialized
        int font(const std::string &path, Font f, int id = -1);

        /// Sets the system font. Do not call after first initialization.
        void set_sys_font(const std::string &path, const Font &fnt);

        /// Actually initializes the frameTables, fonts, & the textures
        void _compose(FrameList& fm, FontList& fl);
    };
}
