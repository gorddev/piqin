#include "engine/types/Quad.hpp"
#include "engine/scene/animation/Frame.hpp"

using namespace geng;

// ................
// Quad Code
// ................

Quad::Quad(int x, int y, int w, int h, float duration, GAnimType anim)
    : x(x), y(y), w(w), h(h), duration(duration), anim(anim) {}

std::vector<SDL_FPoint> Quad::to_vert_points(int texW, int texH) const{
    std::vector<SDL_FPoint> vertexPoints(4);
    vertexPoints[0] = {(x + 0.5f) / texW, (y + 0.5f) / texH}; // top-left
    vertexPoints[1] = {(x + w + 0.01f) / texW, (y + 0.5f) / texH}; // top-right
    vertexPoints[2] = {(x + 0.5f) / texW, (y + h - 0.f) / texH}; // bottom-left
    vertexPoints[3] = {(x + w + 0.01f) / texW, (y + h - 0.0f) / texH}; // bottom-right
    return vertexPoints;
}