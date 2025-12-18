#pragma once

#include <vector>
#include <SDL_rect.h>

#include "FontTransform.hpp"
#include "../../types/positioning/Quad.hpp"
#include <SDL_render.h>

namespace geng {
    /** Stores the information for one character of a font. Has member varibles:
     * - @code std::vector<SDL_FPoint> points@endcode › contains the base points of the font
     * - @code short w, h@endcode › The width and height of the character.
     * **/
    struct FontChar {
        /// Contains all the points on the texture atlas
        std::vector<SDL_FPoint> points;
        /// Contains width, and spacing for each character.
        short w, h, s;
        /// Appends buffer to a buffer for editing.
        void to_vertex(const FontTransform& t, std::vector<SDL_Vertex>& buffer) {
            SDL_Vertex topleft = { {t.pos.x, t.pos.y}, t.color, points[0]};
            SDL_Vertex topright = {{t.pos.x + w * t.scale, t.pos.y}, t.color, points[1]};
            SDL_Vertex bottomleft = { {t.pos.x, t.pos.y + h * t.scale}, t.color, points[2]};
            SDL_Vertex bottomright = {{t.pos.x + w * t.scale, t.pos.y + h * t.scale}, t.color, points[3]};

            buffer.push_back(topleft);
            buffer.push_back(bottomleft);
            buffer.push_back(bottomright);
            buffer.push_back(topleft);
            buffer.push_back(topright);
            buffer.push_back(bottomright);
        }

        /// Converts quad into points
        FontChar(const Quad& quad, const IMG_Info& img, short spacing = 0) {
            points = quad.to_vert_points(img.w, img.h);
            s = spacing;
            w = quad.w;
            h = quad.h;
        }
    };
}
