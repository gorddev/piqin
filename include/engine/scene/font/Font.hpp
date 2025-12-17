#pragma once

#include "engine/utilities/image-info/IMGDecoder.hpp"
#include "FontChar.hpp"
#include <unordered_map>

namespace geng {
    /** @brief Stores all the information for one font be rendered.
     *  @details Member variables:
     *  - @code texture_id@endcode › links the font to a texture
     *  - @code spacing@endcode › default spacing between ecah character in the font. (equals 0)
     *  - @code
     * **/
    struct Font {
    private:
        /// Texture that stores the font
        int texture_id = 0;
        /// Spacing between each font item
        float spacing = 0.f;
    public:
        /// Map between char and the Fontchar
        std::unordered_map<char, FontChar> chars;
        /// Map between each respective quad and its character
        std::unordered_map<char, Quad> quads;

        /** Mandatory constructor for a font:
         * - @code std::string path@endcode › path to the source texture
         * - @code std::unordered_map<char, Quad>@endcode › mapping each character to a position from the source texture
         * - @code short spacing = 0.f@endcode › pt spacing between each letter
         * - @code short offset_x = 0@endcode › how much off from x = 0 your original quad positons are (if you're using a texture atlas)
         * - @code short offset_y = 0@endcode › same thing
         */
        Font(std::unordered_map<char, Quad> quads, short spacing = 0.f, short offset_x = 0, short offset_y = 0);
        Font(const Font& f);

        /// Adds the correct character to a buffer of characters
        void add_FontChar(char c, std::vector<FontChar> &buffer, Vertex pos);

        /// Initializes the font
        void _init(IMG_Info info);

        /// Gets the height of the text
        [[nodiscard]] short get_height() const;
        /// Gets the texture id of the font
        [[nodiscard]] int get_texture_id() const;
        /// stets the texture id of the font
        void set_texture_id(int id);

    };
}

