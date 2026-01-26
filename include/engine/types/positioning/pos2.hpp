#pragma once
#include <SDL_rect.h>

namespace gan {

    struct pos2 {
        int x, y;
        pos2() : x(0), y(0) {}
        pos2(int x, int y) : x(x), y(y) {}
        bool operator==(const pos2& p) const {
            return (x == p.x && y == p.y);
        }
        bool operator!=(const pos2& p) const {
            return (x != p.x || y != p.y);
        }
        pos2 operator+(const pos2& p) const {
            return {x + p.x, y + p.y};
        }
        pos2 operator-(const pos2& p) const {
            return {x - p.x, y - p.y};
        }
        void operator+=(const pos2& p) {
            x += p.x;
            y += p.y;
        }
        void operator-=(const pos2& p) {
            x -= p.x;
            y -= p.y;
        }
        pos2 operator-() const {
            return {-x, -y};
        }

        operator SDL_Point() const { // NOLINT(*-explicit-constructor)
            return {x, y};
        }

        operator SDL_FPoint() const { // NOLINT(*-explicit-constructor)
            return {static_cast<float>(x), static_cast<float>(y)};
        }

        [[nodiscard]] SDL_FPoint to_f_point() const {
            return {static_cast<float>(x), static_cast<float>(y)};
        }
    };
    
}
