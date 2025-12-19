#pragma once
#include <SDL_render.h>
#include <vector>

#include "engine/rendering/shadows/ShadowBank.hpp"
#include "engine/types/positioning/Point2D.hpp"
#include "engine/types/Transform.hpp"

namespace geng {

    class BannerBuffer {
    private:
        /// Contains the buffer of vertices
        std::vector<SDL_Vertex> buffer;
        /// Contains the position of the banner
        Transform& bannerT;
        /// Contains a pointer to the ShadowBank
        ShadowBank* shadows = nullptr;
        /// Contains the white point of each texture
        SDL_FPoint white_point = {1.f, 1.f};
        /// Contains the amount we move each banner element by
        Point2D move;

    public:
        /// Constructor: requires a banner to exist first
        explicit BannerBuffer(Transform& bannerT);

        /// Allows widgets to append themselves to the buffer
        void push_back(SDL_Vertex vertex);
        /// Adds the properties of a vertex to the banner buffer to be added to the vertex buffer
        void push_back(SDL_FPoint& pos, SDL_Color& color, SDL_FPoint& tex_coord);
        /// Adds a point to the buffer -- thus ensuring that it is a single color. Default color is white.
        void push_back(SDL_FPoint pos, SDL_Color color = {255, 255, 255, 255});

        /// Sets the shadow bank of the BannerBuffer
        void _set_shadow_bank(ShadowBank* new_bank);
        /// Sets the wqhite point of the BannerBUffer
        void _set_white_point(SDL_FPoint new_white_point);
        /// Gets the buffer of vertices
        [[nodiscard]] std::vector<SDL_Vertex>& _get_vertex_buffer();
        /// Clears the buffer
        void _clear_buffer();
    };
}

