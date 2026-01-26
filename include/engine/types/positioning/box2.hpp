#pragma once

#include <SDL_rect.h>

namespace gan {

    /** @brief Basic shape that contains an @code int x@endcode , @code int y@endcode, @code short w@endcode, and @code short h@endcode.
     * @details Contains ==, !=, +, - nontrivial operator overloads and an @code int area@endcode function that returns @code w*h@endcode. **/
    struct box2 {
        int x, y;
        short w, h;
        box2() : x(0), y(0), w(0), h(0) {}
        box2(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
        bool operator==(const box2& b) const {
            return (x == b.x && y == b.y && w == b.w && h == b.h);
        }
        bool operator!=(const box2& b) const {
            return (x != b.x || y != b.y || w != b.w || h != b.h);
        }
        box2 operator+(const box2& b) const {
            return {x + b.x, y + b.y, w + b.w, h + b.h};
        }
        box2 operator-(const box2& b) const {
            return {x - b.x, y - b.y, w - b.w, h - b.h};
        }
        void operator+=(const box2& b) {
            x += b.x;
            y += b.y;
            w += b.w;
            h += b.h;
        }
        void operator-=(const box2& b) {
            x -= b.x;
            y -= b.y;
            w -= b.w;
            h -= b.h;
        }
        box2 operator-() const {
            return {-x, -y, -w, -h};
        }
        int area() const { return w*h; }
        /// Converts the box into two repsentative SDL tex_points. Order: topleft, bottomleft, bottomright, topright.
        std::vector<SDL_FPoint> to_points() const {
            std::vector<SDL_FPoint> points;
            SDL_FPoint tl = {x + 0.f, y +0.f};
            SDL_FPoint bl = {x + 0.f, y + h + 0.f};
            SDL_FPoint br = {x + w + 0.f, y + h + 0.f};
            SDL_FPoint tr = {x + w + 0.f, y + 0.f};

            points.push_back(tl);
            points.push_back(bl);
            points.push_back(br);
            points.push_back(tr);
            return points;
        }
        /// Converts the box into 6 representative tex_points that can be used to render the box accordingly, appended to a point buffer.
        void to_tex_points(std::vector<SDL_FPoint>& points) const {
            std::vector<SDL_FPoint> my_points = to_points();
            points.push_back(my_points[0]);
            points.push_back(my_points[1]);
            points.push_back(my_points[2]);
            points.push_back(my_points[0]);
            points.push_back(my_points[2]);
            points.push_back(my_points[3]);
        }
        /// special version which uses the normalized coordinates
        std::vector<SDL_FPoint> to_normalized_coordinates(float texW, float texH) const {
            std::vector<SDL_FPoint> points;
            SDL_FPoint tl = {x / texW, y / texH};
            SDL_FPoint bl = {x / texW, (y + h)/texH};
            SDL_FPoint br = {(x + w)/texW, (y + h)/texH};
            SDL_FPoint tr = {(x + w)/texW, y /texH};

            points.push_back(tl);
            points.push_back(bl);
            points.push_back(br);
            points.push_back(tr);
            return points;
        }
        /// Turns it into a hitbox for rendering, 4*6 vertices in total.
        std::vector<SDL_FPoint> to_hitbox(uint16_t thickness) {

            box2 left = {x, y, thickness, h};
            box2 top = {x +thickness, y, w - thickness, thickness};
            box2 bottom = {x + thickness, y + h - thickness, w - thickness, thickness};
            box2 right = {x + w - thickness, y + thickness, thickness, h - thickness*2};

            std::vector<SDL_FPoint> points;
            left.to_tex_points(points);
            top.to_tex_points(points);
            bottom.to_tex_points(points);
            right.to_tex_points(points);
            return points;
        }
    };
}
