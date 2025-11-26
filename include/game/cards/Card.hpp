#pragma once
#include "generics/Object.hpp"
#include "textures/asset-info/RegistryConstants.hpp"

static const Transform defaultCardTransform = {
    {scene::width/2.0f,0,0},
    {0,0,0},
    48,
    32,
    1,
    Z_HAND_BASE
};

static const FrameState defaultCardFrameState = {
    ASSET_DECK_ID
};

class Card : public Object {
    uint8_t value;
    char suite;
    bool flipped = false;

public:
    Card(uint8_t value, char suite) : value(value), suite(suite) {
        if (value > 13) value = 13;
        char mod = 0;
        if (suite == 'h') mod = 1;
        else if (suite == 'd') mod = 2;
        else if (suite == 'c') mod = 3;
        t = defaultCardTransform;
        fs = defaultCardFrameState;
        fs.state = value + (mod * 13) - 2;
        fs.frameNum = 0;
    }

    Card(Card& c) {
        value = c.value;
        suite = c.suite;
        flipped = c.flipped;
        t= c.t;
        fs = c.fs;
    }

    bool operator==(const Card c) const { return value == c.value && suite == c.suite; }
};
