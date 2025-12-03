#pragma once

#include "engine/objects/Object.hpp"
#include "game/blackjack/cards/Card.hpp"
#include <vector>

#define MAX_DECK_SIZE 100.0f
#define STANDARD_DECK_SIZE 52

namespace blackjack {
    class Deck: public gengine::Object {
    private:
        // All cards in the deck
        std::vector<Card*> drawPile;
    public:
        // Constructor
        Deck();

        ~Deck();

        // Updates the state of the deck sprite
        void update_state();

        void prep_card_for_pop(Card *c);

        // Adds cards to the deck
        void add_card(Card* c);
        void add_cards(std::vector<Card*> cards);
        // Pops random cards off the deck
        Card* pop_card();
        std::vector<Card*> pop_all();

        // Gets the current height of the stack
        [[nodiscard]] short stack_height() const;

        // Gets the pointers for loading into objectManager
        std::vector<Card*>& gather_objects();

        bool empty();

        int size();
    };
}