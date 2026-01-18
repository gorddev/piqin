#pragma once

#include <game/blackjack/BJEnums.hpp>

#include "engine/scene/physics/Collider.hpp"


class Card : public gan::Collider {
        int value;
        blackjack::BJ_Suit suit;
        bool flipped = false;

        // Returns
        [[nodiscard]] uint8_t to_anim_num();

    public:
        // Base constructor
        Card(gan::FrameTable& frame_table, int val, blackjack::BJ_Suit suite);

        // Checks if two cards are equal.
        bool operator==(Card c) const;

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
        blackjack::BJ_Suit get_suit() const;
        // Changes the value on the card by the number
        bool adjust_value(int num);


        // Function for cards with special purposes.
        bool use(Card* c);

        void on_click_release(gan::Pos2D pos) override;
    };

    inline const gan::Transform2D defaultCardTransform2D = {
        {300 / 2.0f, 0},
        31,
        31,
        1
    };

    // Constants for all the different types of cards
#define CARD_FLIPPED 53
