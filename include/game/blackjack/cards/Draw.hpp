#pragma once
#include <vector>
#include "game/blackjack/cards/Card.hpp"

namespace blackjack {
    class Draw {
    private:
        // Holds all the cards in our hand
        std::vector<Card*> draw;
        // Lets us keep track of positions we excavate
        std::vector<short> removals;
        // Position of the draw
        gengine::Vertex pos;
    public:
        // Only one constructor. We specify a position.
        explicit Draw(gengine::Vertex pos);

        // Gets the number of cards in the current draw
        [[nodiscard]] int get_num_cards();

        // Adds a card to the deal & gives it a target path.
        void add_card(Card* card, bool flipped = false);

        // Gets the score of the curent deal. Needs the target to work.
        [[nodiscard]] int get_score() const;
        // Returns if the card will bust
        [[nodiscard]] bool will_bust(Card* c);

        // Pops cards off the Card draws
        Card* pop_card(int cardNum = -1);
        std::vector<Card*> pop_cards();

        // Gathers all the objects in the draw
        std::vector<Card*>& gather_objects();

        gengine::Vertex get_pos();

        int get_index(Card *c);

        // Utility functions
        [[nodiscard]] int size() const;
        [[nodiscard]] bool empty() const;
        void set_pos(gengine::Vertex v) ;
        Card* operator[](int index);
    };
}
