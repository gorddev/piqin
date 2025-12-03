#pragma once
#include "EngineSource.hpp"
#include "../BJEnums.hpp"

namespace blackjack {

    class Card : public gengine::Object {
        int value;
        BJ_Suit suit;
        bool flipped = false;

        // Returns
        [[nodiscard]] uint8_t to_state();

    public:
        // Base constructor
        Card(int val, BJ_Suit suite);

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
        BJ_Suit get_suit() const;
        // Changes the value on the card by the number
        bool adjust_value(int num);


        // Function for cards with special purposes.
        bool use(Card* c);
    };

    inline const gengine::Transform defaultCardTransform = {
        {gengine::glb::scene.width / 2.0f, 0, 0},
        48,
        32,
        1
    };

    inline const gengine::FrameState defaultCardFrameState = {
        ASSET_DECK_ID
    };

    // Constants for all the different types of cards
#define CARD_FLIPPED 53
}