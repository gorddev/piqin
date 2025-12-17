#include "../../../include/engine/types/Random.hpp"

geng::Random& geng::global::rand() {
    static Random r;
    return r;
}