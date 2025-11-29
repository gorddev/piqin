#include "game/blackjack/cards/Card.hpp"

// Adjust the value of a card by a certain amount
bool Card::adjust_value(int num) {
    // Can't change the value of special cards.
    if (suit == 'x') return false;
    // Otherwise continue on as normal
    value = (value + (num%14));
    if (value <= 0) value = 13 + value;
    if (value == 0) value++;
    // Make sure to update the state
    fs.state = to_state();
    return true;
}

// Gets the score of a card.
int Card::get_score() const {
    // If it's a special card, return 0
    if (suit == 'x') return 0;
    if (value > 10) return 10;
    return value;
}

char Card::get_suit() const {
    return suit;
}

