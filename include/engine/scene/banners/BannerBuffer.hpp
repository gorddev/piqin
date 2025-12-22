#pragma once
#include <SDL_render.h>
#include <vector>

#include "engine/rendering/shadows/ShadowBank.hpp"
#include "engine/types/positioning/Pos2D.hpp"
#include "engine/types/Transform2D.hpp"

namespace geng {

    class BannerBuffer {
    private:
        /// Contains the buffer of vertices
        std::vector<SDL_Vertex> buffer;
        /// Contains the position of the banner
        Transform2D& bannerT;
        /// Contains a pointer to the ShadowBank
        ShadowBank* shadows = nullptr;
        /// Contains the white point of each texture
        SDL_FPoint white_point = {1.f, 1.f};
        /// Contains the amount we move each banner element by
        Pos2D move;

    public:
        /// Constructor: requires a banner to exist first
        explicit BannerBuffer(Transform2D& bannerT);

        /// Allows widgets to append themselves to the buffer
        void push_back(SDL_Vertex FPos2D);
        /// Adds the properties of a FPos2D to the banner buffer to be added to the FPos2D buffer
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

