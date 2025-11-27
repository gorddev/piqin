#pragma once

#include "textures/asset-info/RegistryConstants.hpp"
#include "textures/asset-info/SpriteRegistry.hpp"

// Path to item
#define ASSET_CARD_SELECTOR_PATH "assets/card-selector.ktx2"
#define ASSET_CARD_SELECTOR_RENDER_TYPE RENDER_VERTICAL

// Sprite info
static const SpriteSheet asset_card_selector = {
    8,
    8,
    5,
    5,
    ASSET_CARD_SELECTOR_PATH,
    ASSET_CARD_SELECTOR_RENDER_TYPE,
    FRAME_STOP,
    -1
};

// Register our sprite
static SpriteRegister asset_card_selector_obj(asset_card_selector, ASSET_CARD_SELECTOR_ID);