#pragma once
#include "generics/Object.hpp"
#include "textures/asset-info/RegistryConstants.hpp"

class Card : public Object {
    int value;
    char suit;
    bool flipped = false;

    [[nodiscard]] uint8_t to_state();

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

    void increase_value(int num);

    void change_into_card(Card *c);
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

// Constants for all the different types of cards
#define CARD_JOKER
#define CARD_FLIPPED 53