#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>

#include "engine/types/positioning/Pos2D.hpp"
#include "engine/types/AnimBox2D.hpp"

namespace geng {
    /** Stores the information for one character of a font. Has member varibles:
     * - @code lni::vector<SDL_FPoint> points@endcode › contains the base points of the font
     * - @code short w, h@endcode › The width and height of the character.
     * **/
    struct FontChar {
        /// Contains all the points on the texture atlas
        gch::vector<SDL_FPoint> points;
        /// Contains width, and spacing for each character.
        short w, h, s;
        /// Appends buffer to a buffer for editing.
        void to_vertex(gch::vector<SDL_Vertex>& buffer, Pos2D& pos, SDL_Color& color, float& scale) {

            SDL_Vertex topleft = { {pos.x + 0.f, pos.y + 0.f}, color, points[0]};
            SDL_Vertex topright = {{pos.x + w * scale, pos.y + 0.f}, color, points[1]};
            SDL_Vertex bottomleft = { {pos.x + 0.f, pos.y + h * scale}, color, points[2]};
            SDL_Vertex bottomright = {{pos.x + w * scale, pos.y + h * scale}, color, points[3]};

            buffer.push_back(topleft);
            buffer.push_back(bottomleft);
            buffer.push_back(bottomright);
            buffer.push_back(topleft);
            buffer.push_back(topright);
            buffer.push_back(bottomright);
        }

        FontChar() = default;

        /// Constructor converts quad into points
        FontChar(const AnimBox2D& quad, const IMG_Info& img, short spacing = 0) {
            points = quad.to_vert_points(img.w, img.h);
            s = spacing;
            w = quad.w;
            h = quad.h;
        }

        /// to_string
        geng::str_view& to_fstring(geng::str_view& buffer) const {
            for (size_t i = 0; i < points.size(); ++i) {
                buffer << '(' << points[i].x << ", " << points[i].y << ')';
                if (i + 1 < points.size()) buffer << ", ";
            }
            return buffer << "] }";
        }
    };
}
