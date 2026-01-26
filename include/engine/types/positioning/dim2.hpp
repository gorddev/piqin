#pragma once

namespace gan {
    /** @brief Basic dimension struct that contains a @code short w@endcode and @code short h@endcode.
     * @details Contains @code ==, !=, +, -@endcode nontrivial operator overloads and an @code int area()@endcode function that returns @code w*h@endcode.
     * @note Typically used for 2D object's demensions in a limited game environment world.*/
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
}