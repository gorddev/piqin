#include "utilities/Utilities.hpp"

signed char utils::sgn(long double d) {
    const int dd = static_cast<int>(d * 1000.0) / 1000;
    if (dd > 0)
        return 1;
    if (dd < 0)
        return -1;
    return 0;
}

