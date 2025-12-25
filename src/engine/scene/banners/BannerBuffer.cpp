#include "engine/scene/banners/BannerBuffer.hpp"

using namespace geng;

/// Constructor: requires a banner to exist first
BannerBuffer::BannerBuffer(Transform2D& bannerT)
    : bannerT(bannerT), buffer(60) {
}

/// Allows widgets to append themselves to the buffer
void BannerBuffer::push_back(SDL_Vertex FPos2D) {
    FPos2D.position.x += move.x;
    FPos2D.position.y += move.y;
    buffer.push_back(FPos2D);
}

/// Adds the properties of a FPos2D to the banner buffer to be added to the FPos2D buffer
void BannerBuffer::push_back(SDL_FPoint& pos, SDL_Color& color, SDL_FPoint& tex_coord) {
    push_back({pos, color, tex_coord});
}

/// Adds a point to the buffer -- thus ensuring that it is a single color. Default color is white.
void BannerBuffer::push_back(SDL_FPoint pos, SDL_Color color) {
    push_back({ pos, color, white_point });
}

/// Sets the shadow bank of the BannerBuffer
void BannerBuffer::_set_shadow_bank(ShadowBank* new_bank) {
    shadows = new_bank;
}

/// Sets the wqhite point of the BannerBUffer
void BannerBuffer::_set_white_point(SDL_FPoint new_white_point) {
    move.x = static_cast<int>(bannerT.pos.x - (bannerT.w / 2.0f));
    move.y = static_cast<int>(bannerT.pos.y - (bannerT.h / 2.0f));
    white_point = new_white_point;
}

/// Gets the buffer of vertices
gch::vector<SDL_Vertex>& BannerBuffer::_get_vertex_buffer() {
    return buffer;
}

/// Clears the buffer
void BannerBuffer::_clear_buffer() {
    buffer.clear();
    buffer = {};
}