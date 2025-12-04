#pragma once

#include "../RegistryConstants.hpp"
#include "../SheetRegistry.hpp"

// Path to item
#define ASSET_DECK_PATH
// Render type
#define ASSET_DECK_RENDER_TYPE RENDER_VERTICAL

// Sprite info
static const gengine::textures::FrameSheet asset_deck = {
    "assets/kumpu-cards.ktx2",
    TEX_WIDTH,
    TEX_HEIGHT,
    0,0,
    32,48,
    13,65
};

static gengine::textures::SheetRegister asset_deck_obj(ASSET_DECK_ID, asset_deck);