#include "../../../../include/scene/render/RenderManager/RenderManager.hpp"

static void scale_rect(SDL_Rect& rect, const double& scale) {
    rect.x *= scale; // NOLINT(*-narrowing-conversions)
    rect.y *= scale; // NOLINT(*-narrowing-conversions)
    rect.w *= scale; // NOLINT(*-narrowing-conversions)
    rect.h *= scale; // NOLINT(*-narrowing-conversions)
}
/* .....  */
/* CREATE SHADOW */
/* .....  */
SDL_Texture* RenderManager::create_shadow_texture(SDL_Texture* texture) {
    // Width and height of the original texture
    int w, h;
    Uint32 format;
    // Gets info from our original texture
    SDL_QueryTexture(texture, &format, nullptr, &w, &h);

    // Create a target to render to.
    SDL_Texture* shadow = SDL_CreateTexture(
        renderer,
        format,
        SDL_TEXTUREACCESS_TARGET,
        w, h
    );

    // Set our blendmode for shadows
    SDL_SetTextureBlendMode(shadow, SDL_BLENDMODE_BLEND);
    SDL_Texture* oldTarget = SDL_GetRenderTarget(renderer);

    // Render into our shadow
    SDL_SetRenderTarget(renderer, shadow);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);

    // restore the previous target (probably nullptr)
    SDL_SetRenderTarget(renderer, oldTarget);

    // Make the texture all black
    SDL_SetTextureColorMod(shadow, 0, 0, 0);
    // Set the opacity
    SDL_SetTextureAlphaMod(shadow, SHADOW_ALPHA);
    // Add the shadow to our spritesheet for the future
    SDL_SetTextureBlendMode(shadow, SDL_BLENDMODE_BLEND);

    return shadow;
}
