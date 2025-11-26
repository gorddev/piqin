#pragma once

#include <SDL.h>

// Allows us to easily organize objects for rendering.
struct TextureRenderInfo {
    SDL_FRect canvas_rect;
    int sheet_id = 0;
    float rotate = 0;
    SDL_Rect* sheet_rect;
    TextureRenderInfo(SDL_FRect canvas, SDL_Rect* sheet_loc)
        : canvas_rect(canvas), sheet_rect(sheet_loc){ }
    TextureRenderInfo(SDL_FRect canvas, SDL_Rect* sheet_loc, int sheet_id)
        : canvas_rect(canvas), sheet_rect(sheet_loc), sheet_id(sheet_id){ }

};