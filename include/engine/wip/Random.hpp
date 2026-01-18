#pragma once
#include <random>

namespace gan {
    struct Random {
    private:
        std::mt19937 gen;
        std::uniform_real_distribution<float> dfloat;
    public:
        Random() {
            gen = std::mt19937(std::random_device{}());
        }
        int rint(int low, int high) {
            std::uniform_int_distribution<int> dist(low, high);
            return dist(gen);
        }

        float rfloat(float low, float high) {
            std::uniform_real_distribution<float> dist(low, high);
            return dist(gen);
        }
    };
    namespace global {
        Random& rand();
    }
}