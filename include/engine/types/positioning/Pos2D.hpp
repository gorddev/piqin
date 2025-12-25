#pragma once
#include <SDL_rect.h>

namespace geng {

    struct Pos2D {
        int x, y;
        Pos2D() : x(0), y(0) {}
        Pos2D(int x, int y) : x(x), y(y) {}
        bool operator==(const Pos2D& p) const {
            return (x == p.x && y == p.y);
        }
        bool operator!=(const Pos2D& p) const {
            return (x != p.x || y != p.y);
        }
        Pos2D operator+(const Pos2D& p) const {
            return {x + p.x, y + p.y};
        }
        Pos2D operator-(const Pos2D& p) const {
            return {x - p.x, y - p.y};
        }
        void operator+=(const Pos2D& p) {
            x += p.x;
            y += p.y;
        }
        void operator-=(const Pos2D& p) {
            x -= p.x;
            y -= p.y;
        }
        Pos2D operator-() const {
            return {-x, -y};
        }

        operator SDL_Point() const { // NOLINT(*-explicit-constructor)
            return {x, y};
        }

        operator SDL_FPoint() const { // NOLINT(*-explicit-constructor)
            return {static_cast<float>(x), static_cast<float>(y)};
        }

        SDL_FPoint to_f_point() const {
            return {static_cast<float>(x), static_cast<float>(y)};
        }
    };
    
}
