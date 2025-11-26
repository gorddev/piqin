#include "../../../../include/scene/render/RenderManager/RenderManager.hpp"
#include <iostream>
string to_string_RectF(SDL_FRect* rect) {
    return "{" + std::to_string(rect->x) + ", " + std::to_string(rect->y) + ", " + std::to_string(rect->w) + ", " + std::to_string(rect->h) + "}\n";
}

void RenderManager::render_flats(std::unordered_map<int, std::vector<TextureRenderInfo *>> & flats) const {
    // Render all the flats
    for (auto& [sheet_id, texvec]: flats) {

        // Get our texture from the spritesheet
        SDL_Texture* texture = engine::sm->get_sheet_texture(sheet_id);
        // Then go through and render it

        for (const auto& tex: texvec) {
            SDL_RenderCopyF(renderer,
                texture,
                tex->sheet_rect,
                &tex->canvas_rect);
        }
    }
}

void RenderManager::render_verticals(std::vector<TextureRenderInfo> texvec) {
    for (int i = 0; i < texvec.size(); i+=2) {
        // First we render the shadow if it's valid
        if (texvec[i+1].sheet_id >= 0) {
            SDL_RenderCopyF(renderer,
                engine::sm->get_sheet_shadow(texvec[i].sheet_id),
                texvec[i+1].sheet_rect,
                &texvec[i+1].canvas_rect);
        }
        // Then we render the vertical
        SDL_RenderCopyF(renderer,
            engine::sm->get_sheet_texture(texvec[i].sheet_id),
            texvec[i].sheet_rect,
            &texvec[i].canvas_rect);
    }
}