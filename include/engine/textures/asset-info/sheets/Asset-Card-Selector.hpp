#pragma once

#include "../RegistryConstants.hpp"
#include "../SheetRegistry.hpp"


// Sprite info
static const gengine::textures::FrameSheet asset_card_selector = {
    "assets/game-texture.ktx2",
    TEX_WIDTH, TEX_HEIGHT,
    416, 160,
    8,8,
    6,6,
};

// Register our sprite
static gengine::textures::SheetRegister asset_card_selector_obj(ASSET_CARD_SELECTOR_ID, asset_card_selector);