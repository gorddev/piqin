#include "../../../include/engine/wip/Random.hpp"

geng::Random& geng::global::rand() {
    static Random r;
    return r;
}