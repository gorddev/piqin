#pragma once
#include <SDL.h>

// Allows us easy conversions to SDL_Rects for floating point precision
struct Render_Rect {
    double x, y, w, h;
    Render_Rect(double x, double y, double w, double h);
    [[nodiscard]] SDL_Rect to_SDL_Rect() const;
    [[nodiscard]] SDL_FRect to_SDL_FRect() const;
    void operator*=(double scalar);
    Render_Rect	operator*(double scalar);
};