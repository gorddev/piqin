#include "../../../include/engine/wip/Random.hpp"

gan::Random& gan::global::rand() {
    static Random r;
    return r;
}