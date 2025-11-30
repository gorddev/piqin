#pragma once

#include "../RegistryConstants.hpp"
#include "../SpriteRegistry.hpp"

// Path to item
#define ASSET_CARD_SELECTOR_PATH "assets/card-selector.ktx2"

// Sprite info
static const gengine::SpriteSheet asset_card_selector = {
    8,
    8,
    6,
    6,
    ASSET_CARD_SELECTOR_PATH,
    gengine::GENG_Anim::STOP,
    -1
};

// Register our sprite
static gengine::SpriteRegister asset_card_selector_obj(asset_card_selector, ASSET_CARD_SELECTOR_ID);