#pragma once

namespace geng {

    /** @brief Basic Point struct that contains an integer @code int x@endcode & @code int y@endcode.
     * @details Contains @code ==, !=, +, -@endcode nontrivial operator overloads. **/
    struct Point2D;
    /** @brief Basic dimension struct that contains a @code short w@endcode and @code short h@endcode.
     * @details Contains @code ==, !=, +, -@endcode nontrivial operator overloads and an @code int area()@endcode function that returns @code w*h@endcode.
     * @note Typically used for 2D object's demensions in a limited game environment world.*/
    struct Dim2D;
    /** @brief Basic shape that contains an @code int x@endcode , @code int y@endcode, @code short w@endcode, and @code short h@endcode.
     * @details Contains ==, !=, +, - nontrivial operator overloads and an @code int area@endcode function that returns @code w*h@endcode. **/
    struct Box2D;
    /** @brief Basic shape that contains a @code short top@endcode, @code short bottom@endcode, @code short left@endcode, and @code short right@endcode.
     * @details Contains ==, !=, +, - nontrivial operator overloads.
     * @note Typically used for padding along edges.**/
    struct Directional2D;


    struct Point2D {
        int x, y;
        Point2D() : x(0), y(0) {}
        Point2D(int x, int y) : x(x), y(y) {}
        bool operator==(const Point2D& p) const {
            return (x == p.x && y == p.y);
        }
        bool operator!=(const Point2D& p) const {
            return (x != p.x || y != p.y);
        }
        Point2D operator+(const Point2D& p) const {
            return {x + p.x, y + p.y};
        }
        Point2D operator-(const Point2D& p) const {
            return {x - p.x, y - p.y};
        }
        void operator+=(const Point2D& p) {
            x += p.x;
            y += p.y;
        }
        void operator-=(const Point2D& p) {
            x -= p.x;
            y -= p.y;
        }
        Point2D operator-() const {
            return {-x, -y};
        }
    };


    struct Dim2D {
        short w, h;
        Dim2D(int w, int h) : w(w), h(h) {}
        bool operator==(const Dim2D& d) const {
            return (w == d.w && h == d.h);
        }
        bool operator!=(const Dim2D& d) const {
            return (w != d.w || h != d.h);
        }
        Dim2D operator+(const Dim2D& d) const {
            return {static_cast<short>(w + d.w), static_cast<short>(h + d.h)};
        }
        Dim2D operator-(const Dim2D& d) const {
            return {static_cast<short>(w - d.w), static_cast<short>(h - d.h)};
        }
        void operator+=(const Dim2D& d) {
            w += d.w;
            h += d.h;
        }
        void operator-=(const Dim2D& d) {
            w -= d.w;
            h -= d.h;
        }
        Dim2D operator-() const {
            return {-w, -h};
        }
        int area() const { return w*h; }
    };


    struct Box2D {
        int x, y;
        short w, h;
        Box2D(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
        bool operator==(const Box2D& b) const {
            return (x == b.x && y == b.y && w == b.w && h == b.h);
        }
        bool operator!=(const Box2D& b) const {
            return (x != b.x || y != b.y || w != b.w || h != b.h);
        }
        Box2D operator+(const Box2D& b) const {
            return {x + b.x, y + b.y, w + b.w, h + b.h};
        }
        Box2D operator-(const Box2D& b) const {
            return {x - b.x, y - b.y, w - b.w, h - b.h};
        }
        void operator+=(const Box2D& b) {
            x += b.x;
            y += b.y;
            w += b.w;
            h += b.h;
        }
        void operator-=(const Box2D& b) {
            x -= b.x;
            y -= b.y;
            w -= b.w;
            h -= b.h;
        }
        Box2D operator-() const {
            return {-x, -y, -w, -h};
        }
        int area() const { return w*h; }
        /// Converts the box into two repsentative SDL points. Order: topleft, bottomleft, bottomright, topright.
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
        /// Converts the box into 6 representative points that can be used to render the box accordingly, appended to a point buffer.
        void to_vertex_points(std::vector<SDL_FPoint>& points) const {
            std::vector<SDL_FPoint> my_points = to_points();
            points.push_back(my_points[0]);
            points.push_back(my_points[1]);
            points.push_back(my_points[2]);
            points.push_back(my_points[0]);
            points.push_back(my_points[2]);
            points.push_back(my_points[3]);
        }
    };

    struct Directional2D {
        short top, bottom, left, right;
        Directional2D(short top, short bottom, short left, short right)
            : top(top), bottom(bottom), left(left), right(right) {}
        bool operator==(const Directional2D& d) const {
            return (top == d.top && bottom == d.bottom && left == d.left && right == d.right);
        }
        bool operator!=(const Directional2D& d) const {
            return (top != d.top || bottom != d.bottom || left != d.left || right != d.right);
        }
        Directional2D operator+(const Directional2D& d) const {
            return {static_cast<short>(top + d.top), static_cast<short>(bottom + d.bottom), static_cast<short>(left + d.left), static_cast<short>(right + d.right)};
        }
        Directional2D operator-(const Directional2D& d) const {
            return {static_cast<short>(top + d.top), static_cast<short>(bottom + d.bottom), static_cast<short>(left + d.left), static_cast<short>(right + d.left)};
        }
        void operator+=(const Directional2D& d) {
            top += d.top;
            bottom += d.bottom;
            left += d.left;
            right += d.right;
        }
        void operator-=(const Directional2D& d) {
            top -= d.top;
            bottom -= d.bottom;
            left -= d.left;
            right -= d.right;
        }
        Directional2D operator-() {
            return {static_cast<short>(-top), static_cast<short>(-bottom), static_cast<short>(-left), static_cast<short>(-right)};
        }
    };
}