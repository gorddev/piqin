#pragma once

#include <game/cards/single-cards/Card.hpp>


class GameMaster;

namespace card {

    class Deck {
    private:
        /// vector containing all of the cards:
        std::vector<Card*> cards;

        /// GameMaster is a friend class
        friend class GameMaster;

        /// Gathers all the objects
        std::vector<Card*>& gather_objects();
    public:
        /// Initialize the deck once with all the cards.
        Deck(gan::FrameTable& card_table);

        /// Pops a random card off the top of the deck
        Card* pop_card();
        /// Adds a card to the deck
        void add_card(Card *card);
    };
}