#pragma once
#include <vector>

#include "engine/rendering/RenderBuffer.hpp"
#include "engine/types/positioning/Box2D.hpp"
#include "../../types/Quad.hpp"

namespace geng {

    /// @brief A tile is a fundamental unit a tileset contains.
    /// @details Once a tile is made, it can't be transmuted.
    class Tile {
        std::vector<SDL_FPoint> tex_points;
        int size = 4;
    public:
        /// Basic constructor
        Tile(Box2D b, int size, int tex_W, int tex_H) : size(size){
            tex_points = b.to_normalized_coordinates(tex_W, tex_H);
        }

        /// Appends to a renderBuffer given a position
        void to_vertex(RenderBuffer& buffer, SDL_FPoint pos) {
            SDL_Color w = {255, 255, 255, 255};
            auto vertex_to_string = [](const SDL_Vertex& v) -> std::string {
                std::ostringstream ss;
                ss << "{pos=(" << v.position.x << ", " << v.position.y << "), w=" << ", tex_points=[";
                for(int i = 0; i < 4; ++i) {
                    ss << "(" << v.tex_coord.x << ", " << v.tex_coord.y << ")";
                    if(i != 3) ss << ", ";
                }
                ss << "]}";
                return ss.str();
            };
            SDL_Vertex tl = {pos, w, tex_points[0]};
            SDL_Vertex bl = {{pos.x, pos.y +size}, w, tex_points[1]};
            SDL_Vertex br = {{pos.x + size, pos.y + size}, w, tex_points[2]};
            SDL_Vertex tr = {{pos.x + size, pos.y}, w, tex_points[3]};
            buffer.push_back(tl);
            buffer.push_back(bl);
            buffer.push_back(br);
            buffer.push_back(tl);
            buffer.push_back(br);
            buffer.push_back(tr);
        }
    };
}
