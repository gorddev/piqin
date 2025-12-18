#pragma once
#include <SDL_render.h>
#include <vector>

#include "engine/rendering/shadows/ShadowBank.hpp"
#include "engine/types/positioning/Vertex.hpp"

namespace geng {

    class BannerBuffer {
    private:
        /// Contains the buffer of vertices
        std::vector<SDL_Vertex> buffer;
        /// Contains the position of the banner
        Vertex& bannerPos;
        /// Contains a pointer to the ShadowBank
        ShadowBank* shadows = nullptr;
        /// Contains the white point of each texture
    public:
        /// Constructor: requires a banner to exist first
        explicit BannerBuffer(Vertex& bannerPos)
            : bannerPos(bannerPos), buffer(60) {}

        /// Allows widgets to append themselves to the buffer
        void push_back(SDL_Vertex vertex) {
            buffer.push_back(vertex);
        }
        /// Adds the properties of a vertex to the banner buffer to be added to the vertex buffer
        void push_back(SDL_FPoint& pos, SDL_Color& color, SDL_FPoint& tex_coord) {
            push_back({pos, color, tex_coord});
        }
        /// Adds a point to the buffer -- thus ensuring that it is a single color. Default color is white.
        void push_back(SDL_FPoint pos, SDL_Color color = {255, 255, 255, 255}) {
            push_back({ pos, color, white_point});
        }


        /// Sets the shadow bank of the BannerBuffer
        void _set_shadow_bank(ShadowBank* new_bank) {
            shadows = new_bank;
        }
    };
}
