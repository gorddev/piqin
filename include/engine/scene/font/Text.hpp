#pragma once
#include <SDL_render.h>
#include <vector>

#include "Font.hpp"
#include "FontTransform.hpp"

namespace geng {

    /** A text is a set collection of letters combined together. **/
    struct Text {
    private:
        /** Contains the positions of the letters of the string inputted **/
        std::vector<FontChar> characters;
    public:
        /// Contains the offsets for each
        std::vector<FontTransform> transforms;
        /// Texture id of the source texture
        short texture_id = 0;
        /// Position of the text
        Vertex pos;

        /** Mandatory constructor for a piece of text
         * - @code std::string textliteral@endcode › actual text you want to be printed
         * - @code geng::Font font@endcode › the font you want to use
         * - @code geng::Vertex pos@endcode › position of the text */
        Text(const std::string &textliteral, Font font, const Vertex &pos) : pos(pos) {
            characters.reserve(textliteral.length());
            Vertex p = {0.f,0.f,0.f};
            for (int i = 0; i < textliteral.length(); i++) {
                if (textliteral[i] == '\n') {
                    p.y += font.get_height() + font.spacing;
                    p.x = 0;
                    continue;
                }
                font.add_FontChar(textliteral[i], characters, pos);
                p.x += font.spacing + characters.back().w;
            }
        }

        void to_vertex(std::vector<SDL_Vertex>& buffer) {
            for (int i = 0; i < characters.size(); i++) {
                characters[i].to_vertex(transforms[i], buffer);
            }
        }
    };

}
