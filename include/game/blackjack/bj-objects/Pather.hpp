#pragma once

#include "Hand.hpp"
#include "../cards/Card.hpp"
#include "Deck.hpp"
#include "Discard.hpp"

namespace blackjack {
    class Board;

    // Paths all of our cards to each location
    struct Pather {
    private:
        std::vector<Card*> travel_to_deck;
    public:
        Pather() {}

        // Updates traveling cards
        void update(Deck& d);
        // Gives paths and particles to each location
        // Deck
        void move(Card* c, Deck* deck);
        void move(std::vector<Card *> cards, Deck *deck);
        // Discard
        void to_discard(Card* c, Discard* discard, short cardNum = 0);
        void to_discard(Card* c, Discard* discard, gengine::GENG_Path path, short cardNum = 0);
        void to_discard(std::vector<Card *> &cards, Discard* discard);
        // Hand
        bool move(Card *c, Hand &hand);
        void update_hand(Hand &hand);
        // Player
        void move(Card* c, Draw& d, int index = -1, bool down = false);
        void move(std::vector<Card *> &cards, Draw &draw);

        void to_floater(Card* c);
    };

    inline Pather pather;
};