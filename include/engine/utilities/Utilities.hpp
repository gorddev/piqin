#pragma once
#include <vector>

namespace gengine::utils {

    static constexpr double pi = 3.14159265358979323846;
    static constexpr double roottwo = 1.4142135624;

    // Returns the sign of a variable to 4 digits (-1, 0, 1)
    signed char sgn(long double);
}
