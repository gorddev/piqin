#pragma once
#include <random>

namespace gengine {
    struct Random {
        std::mt19937 gen;
        std::uniform_real_distribution<float> dfloat;
        Random() {
            gen = std::mt19937(std::random_device{}());
            dfloat = std::uniform_real_distribution<float>(-9999999, 9999999);
        }
        int rint() {
            return gen();
        }
        float rfloat() {
            return dfloat(gen);
        }
    };
    inline Random GENG_Rand;
}