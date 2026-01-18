#pragma once

#include "FontChar.hpp"
#include <SDL2/SDL_ttf.h>

#include "engine/types/external/vector.hpp"
#include "engine/types/positioning/Dim2D.hpp"

struct FontMetrics {
    int ascent = -1;
    int descent = -1;
    int lineSkip = -1;
    int glyphW = -1;
    int glyphH = -1;
    int advance = -1;
};

namespace gan {
    /** @brief Stores all the information for one font be rendered.
     *  @details Member variables:
     *  - @code texture_id@endcode › links the font to a texture
     *  - @code spacing@endcode › default spacing between ecah character in the font. (equals 0)
     *  - @code
     * **/
    struct Font {
    private:
        /// Texture that stores the font_id
        int texture_id = 0;
        /// Storage of the font
        TTF_Font* font = nullptr;
        /// Keeps track fo font metrics
        FontMetrics metrics;
        /// String of the font
        hstring path;
        /// Spacing between each font item
        int padding = 1.f;
        /// Point
        int pt;
    public:
        /// Map between char and the Fontchar
        std::vector<FontChar> chars;

        /** Mandatory constructor for a font:
         * - @code std::string path@endcode › path to the source texture
         * - @code std::unordered_map<char, Quad>@endcode › mapping each character to a position from the source texture
         * - @code short spacing = 0.f@endcode › pt spacing between each letter
         * - @code short offset_x = 0@endcode › how much off from x = 0 your original quad positons are (if you're using a texture atlas)
         * - @code short offset_y = 0@endcode › same thing
         */
        Font (hstring path, uint16_t pt, uint16_t padding = 1);

        Font(int gw, int gh, int tw, int th, int pad, int texid);

        /// Adds the correct FontChar to a buffer of FontChars given a character c.
        /// Adds one character to the font
        void add_char(char c, Dim2D atlas_dim, Pos2D tex_pos, Dim2D tex_dim, uint16_t
                      advance);

        /// Gets the texture id of the font
        [[nodiscard]] int get_texture_id() const;
        /// Gets the spacing of the font
        [[nodiscard]] int get_padding() const;
        /// Gets the point of the font
        [[nodiscard]] int get_pt() const;
        /// Returns true if the font has the character
        [[nodiscard]] bool has_character(char c);
        /// stets the texture id of the font
        void set_texture_id(int id);
        /// gets a specific fontchar from the map
        FontChar* get_char(char c);
        /// gets a width of a cahracter
        uint16_t get_width(char c);
        /// gets the height of a character
        uint16_t get_height(char c);
        /// gets font metrics
        FontMetrics get_metrics();

        /// Adds one character to a vector of FontChars
        bool add_char_to_buffer(char c, gch::vector<FontChar> &buffer);

        /// Puts the font into a string format
        str_view& to_fstring(str_view &buffer);

        /// Gets a given font
        TTF_Font* get_font() const;

    };
}

