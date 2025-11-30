#pragma once

#include "../SpriteRegistry.hpp"
#include "../RegistryConstants.hpp"

#define ASSET_DRAW_DECK_PATH "assets/card-stack.ktx2"
#define ASSET_DRAW_DECK_DRAW_TYPE RENDER_VERTICAL

static const gengine::SpriteSheet asset_draw_deck = {
    37,
    53,
    7,
    7,
    ASSET_DRAW_DECK_PATH,
    gengine::GENG_Anim::STOP,
    -1
};

static gengine::SpriteRegister asset_draw_deck_obj(asset_draw_deck, ASSET_CARD_STACK_ID);