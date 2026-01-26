#pragma once

#include <SDL2/SDL.h>

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow("My window", 0, 0, 500,500, SDL_RENDERER_SOFTWARE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    return 0;
}