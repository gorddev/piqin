#pragma once

#include <utility>
#include "engine/types/external/vector.hpp"

namespace gan {
    struct BackgroundPalette {
        short sumWeights = 0;
        uint8_t numColors;
        uint8_t range;
        uint8_t speed;
        uint8_t chaos = 1;
        gch::vector<uint8_t> colors;

        BackgroundPalette() = default;

        BackgroundPalette (uint8_t numColors, uint8_t range, uint8_t speed, uint8_t chaos, gch::vector<uint8_t> weights, gch::vector<uint8_t> colors)
            : numColors(numColors), range(range), speed(speed), chaos(chaos),
              colors(std::move(colors)) {
            // If our weights aren't enough
            if (weights.size() < numColors) {
                int oldsize = weights.size();
                weights.resize(colors.size());
                for (int i = oldsize; i < colors.size(); i++)
                    weights[i] = 1;
            }
        };
    };

    const BackgroundPalette defaultBackgroundPalette  = {
        8,
        1,
        2,
        20,
        {30,30, 30, 30, 30},
        {
            120, 180, 120, 255,
            118, 178, 118, 255,
            116, 176, 116, 255,
            114, 174, 114, 255,
            111, 171, 111, 255,
            107, 167, 107, 255,
            104, 164, 104, 255,
            102, 162, 102, 255,
            100, 168, 100, 255
        }
    };

    const BackgroundPalette erer = {
        10,
        2,
        2,
        100,
        {30, 30, 30, 30, 30, 30, 30, 30, 30, 30},
        {255, 255, 255, 255,
            240, 240, 240, 255,
            200, 200, 200, 255,
            170, 170, 170, 255,
        140, 140, 140, 255,
        110, 110, 110, 255,
        80, 80, 80, 255,
        50, 50, 50, 255,
        30, 30, 30, 255,
        15, 15, 15, 255}
    };

    const BackgroundPalette tempPal = {
        5,
        10,
        1,
        12,
        {1,30, 30, 30, 30},
        {
            40, 40, 40, 255, //white (kinda)
            0, 0, 0, 255,    //black
            10, 10, 10, 255,
            15, 15, 15, 255,
            12, 12, 12, 255,
        }
    };
}
