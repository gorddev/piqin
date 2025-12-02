#include "game/blackjack/cards/Discard.hpp"

using namespace blackjack;

// We will have three rows of our discard cards during selection.
Discard::Discard() :
        Card(0, BJ_Suit::SPECIAL),
        discardFlay(3),
        menu(discardFlay, true, true) {
    hidden = true;
    set_pos(BJ_DEFAULT_DISCARD_POS);
}

void Discard::add_card(Card *c, int cardNum) {
    discardPile.push_back(c);
}

void Discard::add_cards(std::vector<Card *> cards) {
    for (auto c : cards)
        add_card(c);
}

Card* Discard::pop_card() {
    Card* c = discardPile.back();
    discardPile.pop_back();
    return c;
}

std::vector<Card*> Discard::pop_all() {
    std::vector<Card*> temp = discardPile;
    discardPile.clear();
    return temp;
}

std::vector<Card*>& Discard::gather_objects() {
    return discardPile;
}

int Discard::size() {
    return discardPile.size();
}

bool Discard::empty() {
    return discardPile.empty();
}
