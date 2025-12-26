#pragma once
#include <SDL_pixels.h>

namespace geng {

    // primary colors
    constexpr SDL_Color color_red = {255, 60, 60, 255};
    constexpr SDL_Color color_orange = {255, 165, 0, 255};
    constexpr SDL_Color color_yellow = {255, 255, 60, 255};
    constexpr SDL_Color color_green = {100, 200, 100, 255};
    constexpr SDL_Color color_blue = {120, 170, 255, 255};
    constexpr SDL_Color color_purple = {210, 90, 255, 255};
    // extra colors
    constexpr SDL_Color color_light_blue = {140, 235, 255, 255};
    constexpr SDL_Color color_pink = {255, 170, 170, 255};
    constexpr SDL_Color color_white = {255, 255, 255, 255};
    constexpr SDL_Color color_black = {20, 20, 20, 255};
    // special colors
    constexpr SDL_Color debug_grey = {0, 0, 0, 150};

}
