#pragma once

namespace gan {
    /** @brief Basic shape that contains a @code short top@endcode, @code short bottom@endcode, @code short left@endcode, and @code short right@endcode.
     * @details Contains ==, !=, +, - nontrivial operator overloads.
     * @note Typically used for padding along edges.**/
    struct Direction2D {
        short top, bottom, left, right;
        Direction2D(short top, short bottom, short left, short right)
            : top(top), bottom(bottom), left(left), right(right) {}
        bool operator==(const Direction2D& d) const {
            return (top == d.top && bottom == d.bottom && left == d.left && right == d.right);
        }
        bool operator!=(const Direction2D& d) const {
            return (top != d.top || bottom != d.bottom || left != d.left || right != d.right);
        }
        Direction2D operator+(const Direction2D& d) const {
            return {static_cast<short>(top + d.top), static_cast<short>(bottom + d.bottom), static_cast<short>(left + d.left), static_cast<short>(right + d.right)};
        }
        Direction2D operator-(const Direction2D& d) const {
            return {static_cast<short>(top + d.top), static_cast<short>(bottom + d.bottom), static_cast<short>(left + d.left), static_cast<short>(right + d.left)};
        }
        void operator+=(const Direction2D& d) {
            top += d.top;
            bottom += d.bottom;
            left += d.left;
            right += d.right;
        }
        void operator-=(const Direction2D& d) {
            top -= d.top;
            bottom -= d.bottom;
            left -= d.left;
            right -= d.right;
        }
        Direction2D operator-() {
            return {static_cast<short>(-top), static_cast<short>(-bottom), static_cast<short>(-left), static_cast<short>(-right)};
        }
    };
}