#pragma once

#include "engine/input/menu/MenuRegion.hpp"
#include "game/blackjack/cards/Card.hpp"

namespace blackjack {

    class Discard : public Card {
    private:
        // Holds all of our discard cards
        std::vector<Card*> discardPile;
        // Holds the rows of discard cards
        std::vector<std::vector<Card*>*> discardFlay;
        // Contains the menu for our discard pile
        gengine::MenuRegion<Card> menu;
    public:
        Discard();

        // Adds card to the discard pile
        void add_card(Card* c, int cardNum = 0);
        void add_cards(std::vector<Card*> cards);

        // Pops cards off the discard pile
        Card* pop_card();
        std::vector<Card*> pop_all();

        // Gathers all objects in the discard pile
        std::vector<Card*>& gather_objects();
        // Gets the size of the discard piel
        int size();

        bool empty();
    };

}