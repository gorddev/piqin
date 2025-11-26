#include "render/Render_Rect.hpp"

Render_Rect::Render_Rect(double x, double y, double w, double h)
    : x(x), y(y), w(w), h(h) {}

[[nodiscard]] SDL_Rect Render_Rect::to_SDL_Rect() const {
    return {static_cast<int>(round(x)),
        static_cast<int>(round(y)),
        static_cast<int>(round(w)),
        static_cast<int>(round(h))};
}

SDL_FRect Render_Rect::to_SDL_FRect() const {
    return {static_cast<float>(x),
        static_cast<float>(y),
        static_cast<float>(w),
        static_cast<float>(h)};
}

void Render_Rect::operator*=(double scalar) {
    x *= scalar;
    y *= scalar;
    w *= scalar;
    h *= scalar;
}
Render_Rect	Render_Rect::operator*(double scalar) {
    return {x * scalar, y * scalar, w * scalar, h * scalar};
}
