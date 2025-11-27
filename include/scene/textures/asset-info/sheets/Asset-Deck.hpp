#pragma once

#include "textures/asset-info/RegistryConstants.hpp"
#include "textures/asset-info/SpriteRegistry.hpp"

// Path to item
#define ASSET_DECK_PATH "assets/kumpu-cards.ktx2"
// Render type
#define ASSET_DECK_RENDER_TYPE RENDER_VERTICAL

// Sprite info
static const SpriteSheet asset_deck = {
    32,
    48,
    13,
    60,
    ASSET_DECK_PATH,
    ASSET_DECK_RENDER_TYPE,
    FRAME_STOP,
    -1
};

static SpriteRegister asset_deck_obj(asset_deck, ASSET_DECK_ID);