#pragma once

#include "../SheetRegistry.hpp"
#include "../RegistryConstants.hpp"

#define ASSET_DRAW_DECK_PATH
#define ASSET_DRAW_DECK_DRAW_TYPE RENDER_VERTICAL

static const gengine::textures::FrameSheet asset_draw_deck = {
    "assets/game-texture.ktx2",
    TEX_WIDTH, TEX_HEIGHT,
    416, 0,
    37, 53,
    2, 6,
};

static gengine::textures::SheetRegister asset_draw_deck_obj(ASSET_CARD_STACK_ID, asset_draw_deck);