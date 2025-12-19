#include "engine/scene/banners/BannerBuffer.hpp"

using namespace geng;

/// Constructor: requires a banner to exist first
BannerBuffer::BannerBuffer(Transform& bannerT)
    : bannerT(bannerT), buffer(60) {
}

/// Allows widgets to append themselves to the buffer
void BannerBuffer::push_back(SDL_Vertex vertex) {
    vertex.position.x += move.x;
    vertex.position.y += move.y;
    buffer.push_back(vertex);
}

/// Adds the properties of a vertex to the banner buffer to be added to the vertex buffer
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
std::vector<SDL_Vertex>& BannerBuffer::_get_vertex_buffer() {
    return buffer;
}

/// Clears the buffer
void BannerBuffer::_clear_buffer() {
    buffer.clear();
    buffer = {};
}