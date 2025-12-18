#pragma once

namespace geng {

    /** Basic Point that contains an @code x@endcode & @code y@endcode **/
    struct Point {
        int x, y;
        Point(int x, int y) : x(x), y(y) {}
    };

    /** Basic dimension that contains a @code w@endcode and @code h@endcode */
    struct Dimension {
        int w, h;
        Dimension(int w, int h) : w(w), h(h) {}
    };

    /** Basic shape that contains an @code x@endcode , @code y@endcode, @code w@endcode, and @code h@endcode **/
    struct Box {
        int x, y, w, h;
        Box(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
    };

    /** Basic shape that contains a @code top@endcode, @code bottom@endcode, @code left@endcode, and @code right@endcode **/
    struct Directional {
        short top, bottom, left, right;
        Directional(short top, short bottom, short left, short right)
            : top(top), bottom(bottom), left(left), right(right) {}
    };
}