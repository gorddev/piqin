#pragma once
#include <vector>

namespace gengine::utils {

    static constexpr float pi = 3.14159265358;
    static constexpr float degreesToRadians = (pi / 180.0f);
    static constexpr float roottwo = 1.4142135624;

    // Returns the sign of a variable to 4 digits (-1, 0, 1)
    signed char sgn(long double);
}
