#pragma once
#include <vector>
#include "game/cards/Card.hpp"

#define DEAL_Z_BASE 8.0f
class Deal {
private:
    // Holds all the cards in our hand
    std::vector<Card*> deal;
    // Position of the deal
    Vertex pos;
    // Gets the path a card takes to the deal.
    Path get_deal_path(Card *&c, float speed = 0.5);
public:
    Deal();
    Deal(Vertex pos);

    // Gets the number of cards in the current deal
    [[nodiscard]] int get_num_cards();
    // Adds a card to the deal & gives it a target path.
    void add_card(Card* card, bool flipped);
    // Adds a card with a given path speed
    void add_card(Card* card, double speed = 0.5);
    // Pops all the cards off the vector
    std::vector<Card*> pop_cards();
    // Pops a single card off the vector
    Card* pop_card(int cardNum = -1);
    // Gets the score of the curent deal
    [[nodiscard]] int get_score(int target = 21) const;

};
