#pragma once
#include "cards/CardSelector.hpp"
#include "game/blackjack/cards/Card.hpp"
#include "game/blackjack/cards/Draw.hpp"

#define HAND_Z_BASE 10.0f
#define HAND_DEFAULT_SPEED 1.0f
namespace blackjack {
    // The hand someone can play cards from
    class Hand {
    private:
        // The cards they have access to
        std::vector<Card*> cards;
        // Whether the hand is flayed for selection or not
    public:
        bool flayed = false;
        // Maintains the last target.
        short lastTarget;

        Hand();
        /* Adding/removing cards */
        // Checks if the hand is full (can check for stackable cards)
        [[nodiscard]] bool full(uint8_t cardType = 0) const;
        [[nodiscard]] bool empty() const;
        [[nodiscard]] int size() const;
        // Adds a card to the hand if there's space
        [[nodiscard]] bool add_card(Card* c);

        // Removes a card with the given cardnum
        Card* pop_card(int cardNum);

        bool has_normal_cards();

        /* Operators */
        Card* operator[](int index) const;
        int get_index(Card *c);

        std::vector<Card *> &gather_objects();
    };
}