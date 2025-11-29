#pragma once
#include <vector>
#include "game/blackjack/cards/Card.hpp"

class CardDraw {
private:
    // Holds all the cards in our hand
    std::vector<Card*> draw;
    // Lets us keep track of positions we excavate
    std::vector<short> removals;
    // Position of the draw
    Vertex pos;
    // Gets the path a card takes to the draw.
    Path get_draw_path(Card *&c, float speed = 0.5);
public:
    // Only one constructor. We specify a position.
    explicit CardDraw(Vertex pos);

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

    // Utility functions
    int size() const;
    bool empty() const;
    Card* operator[](int index);
};
