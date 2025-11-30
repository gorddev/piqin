#pragma once

#include "../RegistryConstants.hpp"
#include "../SpriteRegistry.hpp"

// Path to item
#define ASSET_DECK_PATH "assets/kumpu-cards.ktx2"
// Render type
#define ASSET_DECK_RENDER_TYPE RENDER_VERTICAL

// Sprite info
static const gengine::SpriteSheet asset_deck = {
    32,
    48,
    13,
    60,
    ASSET_DECK_PATH,
    gengine::GENG_Anim::STOP,
    -1
};

static gengine::SpriteRegister asset_deck_obj(asset_deck, ASSET_DECK_ID);