#pragma once
#include <SDL_render.h>
#include <vector>

#include "engine/EngineContext.hpp"

namespace geng {
    static void GENG_cell_flatten_sdl_fpoint(SDL_FPoint& p) {
        if (p.x > 1.0f) p.x = 1.0f;
        if (p.y > 1.0f) p.y = 1.0f;
        if (p.x < 0.0f) p.x = 0.0f;
        if (p.y < 0.0f) p.y = 0.0f;
    }
    /** A Cell is a simple rendering tool that allows for quick testing of textures, ect. They are not efficient and should not be used for general design.**/
    struct Cell {
        int texture_id;
        /// Normalized texture coordinate
        SDL_FPoint tex_start;
        SDL_FPoint tex_end;
        /// Default constructor for a Cell
        Cell(int tex_id, SDL_FPoint tex_start = {0.f,0.f}, SDL_FPoint tex_end = {1.f,1.f})
            : texture_id(tex_id), tex_start(tex_start), tex_end(tex_end) {
            GENG_cell_flatten_sdl_fpoint(tex_start);
            GENG_cell_flatten_sdl_fpoint(tex_end);
        }
        /// Turns a cell into a set of renderable vertices
        int to_vertex(RenderBuffer& buffer, EngineContext& ctx) {
            SDL_FPoint ts = tex_start;
            SDL_FPoint te = {1.f,1.f};
            /// Scene top left
            SDL_FPoint stl = {0.f, 0.f};
            /// Scene bottom right
            SDL_FPoint sbr = {static_cast<float>(ctx.get_width()), static_cast<float>(ctx.get_height())};
            /// White
            SDL_Color w = {255,255,255, 255};

            SDL_Vertex topleft = {stl, w, ts};
            SDL_Vertex bottomleft = {{stl.x, sbr.y}, w, {ts.x, te.y}};
            SDL_Vertex topright = {{sbr.x, stl.x}, w, {te.x, ts.y}};
            SDL_Vertex bottomright = {sbr, w, te};

            buffer.push_back(topleft);
            buffer.push_back(bottomleft);
            buffer.push_back(bottomright);
            buffer.push_back(topleft);
            buffer.push_back(topright);
            buffer.push_back(bottomright);

            return 6;
        }

    };
}
