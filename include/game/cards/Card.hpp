#pragma once
#include "generics/Object.hpp"
#include "textures/asset-info/RegistryConstants.hpp"

#define CARD_FLIPPED 53

class Card : public Object {
    int value;
    char suit;
    bool flipped = false;

    [[nodiscard]] uint8_t to_state() ;

public:
    Card(int val, char suite);

    // Copy constructor
    Card(Card& c);

    bool operator==(const Card c) const;

    void flip();

    void flip_up();

    void flip_down();
    [[nodiscard]] bool is_flipped() const;

    // Gets the card score
    int get_score() const;
};

inline const Transform defaultCardTransform = {
    {scene::width / 2.0f, 0, 0},
    48,
    32,
    1,
    0.0f
};

inline const FrameState defaultCardFrameState = {
    ASSET_DECK_ID
};