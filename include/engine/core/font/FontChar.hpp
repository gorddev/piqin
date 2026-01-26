#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>
#include <vector>

#include "engine/types/strings/hstring/hstring.hpp"
#include "engine/types/positioning/pos2.hpp"
#include "engine/types/positioning/dim2.hpp"

namespace gan {
    /** Stores the information for one character of a font. Has member varibles:
     * - @code lni::vector<SDL_FPoint> points@endcode › contains the base points of the font
     * - @code short w, h@endcode › The width and height of the character.
     * **/
    struct FontChar {
        dim2 dim = {0,0};
        /// Contains all the points on the texture atlas
        SDL_FPoint points[4]{};
        /// How much to advance after placement of this character
        uint16_t advance = 0;
        /// The character this fontchar represents
        char character = '~';

        /// Appends buffer to a buffer for editing.
        void to_vertex(std::vector<SDL_Vertex>& buffer, pos2& pos, SDL_Color& color, int& scale) {
            SDL_Vertex topleft = { {static_cast<float>(pos.x), static_cast<float>(pos.y)}, color, points[0]};
            SDL_Vertex topright = {{static_cast<float>(pos.x) + static_cast<float>(dim.w * scale)*1.f,  static_cast<float>(pos.y)}, color, points[1]};
            SDL_Vertex bottomleft = { {static_cast<float>(pos.x), static_cast<float>(pos.y + dim.h * scale)*1.f}, color, points[2]};
            SDL_Vertex bottomright = {{static_cast<float>((pos.x + dim.w)*scale)*1.f, static_cast<float>((pos.y + dim.h) * scale)*1.f}, color, points[3]};
            buffer.push_back(topleft);
            buffer.push_back(bottomleft);
            buffer.push_back(bottomright);
            buffer.push_back(topleft);
            buffer.push_back(topright);
            buffer.push_back(bottomright);
        }

        FontChar() : dim({0, 0}), points{}, advance(0),
                     character('~') {
        };

        /// Constructor converts quad into points
        FontChar(std::vector<SDL_FPoint>& tex_points, dim2 dim, const uint16_t advance) : dim(dim), advance(advance) {
            points[0] = tex_points[0];
            points[1] = tex_points[1];
            points[2] = tex_points[2];
            points[3] = tex_points[3];
        }

        /// to_string
        gan::str_view& to_fstring(gan::str_view& buffer) const {
            for (size_t i = 0; i < 4; ++i) {
                buffer << '(' << points[i].x << ", " << points[i].y << ')';
                if (i + 1 < 4) buffer << ", ";
            }
            return buffer << "] }";
        }
    };
}
