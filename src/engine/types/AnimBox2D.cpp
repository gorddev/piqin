#include "../../../include/engine/types/AnimBox2D.hpp"

#include <sstream>
#include "engine/scene/animation/Frame.hpp"

using namespace geng;

// ................
// AnimBox2D Code
// ................

AnimBox2D::AnimBox2D(int x, int y, int w, int h, float duration, GAnimType anim, short next_anim)
    : x(x), y(y), w(w), h(h), duration(duration), anim(anim), next_anim(next_anim) {}

gch::vector<SDL_FPoint> AnimBox2D::to_vert_points(int texW, int texH) const{
    gch::vector<SDL_FPoint> vertexPoints(4);
    vertexPoints[0] = {(x + 0.5f) / texW, (y + 0.5f) / texH}; // top-left
    vertexPoints[1] = {(x + w + 0.01f) / texW, (y + 0.5f) / texH}; // top-right
    vertexPoints[2] = {(x + 0.5f) / texW, (y + h - 0.f) / texH}; // bottom-left
    vertexPoints[3] = {(x + w + 0.01f) / texW, (y + h - 0.0f) / texH}; // bottom-right
    return vertexPoints;
}

std::string AnimBox2D::to_string() const {
    std::stringstream ss;
    ss << "Width: " << w << " Height: " << h << " X:" << x << " Y:" << y;
    return ss.str();
}
