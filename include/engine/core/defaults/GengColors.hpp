#pragma once
#include <SDL_pixels.h>

namespace gan {

    // maximum alpha
    constexpr uint8_t max_alpha = 255;

    // primary colors
    constexpr SDL_Color color_red = {gan::max_alpha, 60, 60, gan::max_alpha};
    constexpr SDL_Color color_orange = {gan::max_alpha, 165, 0, gan::max_alpha};
    constexpr SDL_Color color_yellow = {gan::max_alpha, gan::max_alpha, 60, gan::max_alpha};
    constexpr SDL_Color color_green = {100, 200, 100, gan::max_alpha};
    constexpr SDL_Color color_blue = {120, 170, gan::max_alpha, gan::max_alpha};
    constexpr SDL_Color color_purple = {210, 90, gan::max_alpha, gan::max_alpha};
    // extra colors
    constexpr SDL_Color color_light_blue = {140, 235, gan::max_alpha, gan::max_alpha};
    constexpr SDL_Color color_pink = {gan::max_alpha, 170, 170, gan::max_alpha};
    constexpr SDL_Color color_white = {gan::max_alpha, gan::max_alpha, gan::max_alpha, gan::max_alpha};
    constexpr SDL_Color color_black = {20, 20, 20, gan::max_alpha};
    // special colors
    constexpr SDL_Color debug_grey = {0, 0, 0, 150};



}
