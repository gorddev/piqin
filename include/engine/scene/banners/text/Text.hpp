#pragma once
#include <SDL_render.h>
#include <vector>
#include <string>

#include "TextParser.hpp"
#include "engine/scene/font/Font.hpp"
#include "engine/scene/banners/widgets/Widget.hpp"

namespace geng {
    /** A text is a set collection of letters combined together. **/
    class Text : public Widget {
    private:
        /** Contains the positions of the letters of the string inputted **/
        std::vector<FontChar> characters;
        /// Contains the raw text of the font.
        std::string textliteral;
        /** Contains a reference to the original font **/
        Font& font;
        /// Allows text to have duration-based effects
        float time = 0;
        /** Keeps track of vertices so we don't need to re-do them every time **/
        std::vector<SDL_Vertex> vertices;

        void update_character_reserve(const std::string& string_text);

    public:
        /** Mandatory constructor for a piece of text
         * - @code std::string textliteral@endcode › actual text you want to be printed
         * - @code geng::Font font@endcode › the font you want to use
         * - @code geng::FPos2D pos@endcode › position of the text */
        Text(const std::string& string_text, Font& font);

        /** Changes nothing about the text (for now) **/
        void change_dim(Dim2D dimensions) override;

        /** Changes the text of a text object **/
        int to_vertex(BannerBuffer& buffer) override;

        /// to_string function
        std::string to_string() const;
    };
}