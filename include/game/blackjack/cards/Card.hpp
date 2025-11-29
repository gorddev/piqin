#pragma once
#include "generics/Object.hpp"
#include "textures/asset-info/RegistryConstants.hpp"

class Card : public Object {
    int value;
    char suit;
    bool flipped = false;

    // Returns
    [[nodiscard]] uint8_t to_state();

public:
    // Base constructor
    Card(int val, char suite);
    // Copy constructor
    Card(Card& c);

    // Checks if two cards are equal.
    bool operator==(const Card c) const;

    // Flips a card from its current flip
    void flip();
    // Flips a card up if its down
    void flip_up();
    // Fips a card down if its up
    void flip_down();
    // Returns true if flipped down.
    [[nodiscard]] bool is_flipped() const;
    // Lets us see the true value of the card
    int get_value() const;

    bool special() const;

    // Gets the card score
    int get_score() const;
    // Gets the suite
    char get_suit() const;
    // Changes the value on the card by the number
    bool adjust_value(int num);


    // Function for cards with special purposes.
    bool use(Card* c);
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