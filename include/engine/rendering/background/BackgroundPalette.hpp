#pragma once

#include <utility>
#include <vector>
#include "engine/core/defaults/GengColors.hpp"

namespace gan {
    struct BackgroundPalette {
        short sumWeights = 0;
        uint8_t numColors;
        uint8_t range;
        uint8_t speed;
        uint8_t chaos = 1;
        std::vector<uint8_t> colors;

        BackgroundPalette() = default;

        BackgroundPalette (uint8_t numColors, uint8_t range, uint8_t speed, uint8_t chaos, std::vector<uint8_t> weights, std::vector<uint8_t> colors)
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
            120, 180, 120, gan::max_alpha,
            118, 178, 118, gan::max_alpha,
            116, 176, 116, gan::max_alpha,
            114, 174, 114, gan::max_alpha,
            111, 171, 111, gan::max_alpha,
            107, 167, 107, gan::max_alpha,
            104, 164, 104, gan::max_alpha,
            102, 162, 102, gan::max_alpha,
            100, 168, 100, gan::max_alpha
        }
    };

    const BackgroundPalette erer = {
        10,
        2,
        2,
        100,
        {30, 30, 30, 30, 30, 30, 30, 30, 30, 30},
        {gan::max_alpha, gan::max_alpha, gan::max_alpha, gan::max_alpha,
            240, 240, 240, gan::max_alpha,
            200, 200, 200, gan::max_alpha,
            170, 170, 170, gan::max_alpha,
        140, 140, 140, gan::max_alpha,
        110, 110, 110, gan::max_alpha,
        80, 80, 80, gan::max_alpha,
        50, 50, 50, gan::max_alpha,
        30, 30, 30, gan::max_alpha,
        15, 15, 15, gan::max_alpha}
    };

    const BackgroundPalette tempPal = {
        5,
        10,
        1,
        12,
        {1,30, 30, 30, 30},
        {
            40, 40, 40, gan::max_alpha, //white (kinda)
            0, 0, 0, gan::max_alpha,    //black
            10, 10, 10, gan::max_alpha,
            15, 15, 15, gan::max_alpha,
            12, 12, 12, gan::max_alpha,
        }
    };
}
