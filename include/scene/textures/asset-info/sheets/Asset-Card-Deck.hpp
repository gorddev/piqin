#pragma once

#include "textures/asset-info/SpriteRegistry.hpp"
#include "textures/asset-info/RegistryConstants.hpp"

#define ASSET_DRAW_DECK_PATH "assets/card-deck.ktx2"
#define ASSET_DRAW_DECK_DRAW_TYPE RENDER_FLAT

static const SpriteSheet asset_draw_deck = {
    37,
    53,
    6,
    6,
    ASSET_DRAW_DECK_PATH,
    ASSET_DRAW_DECK_DRAW_TYPE,
    FRAME_STOP,
    -1
};

static SpriteRegister asset_draw_deck_obj(asset_draw_deck, ASSET_DRAW_DECK_ID);