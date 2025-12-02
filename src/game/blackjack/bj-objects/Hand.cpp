#include "game/blackjack/Hand.hpp"

#include <iostream>

#include "game/blackjack/BJEnums.hpp"

using namespace blackjack;

Hand::Hand() : lastTarget(0) {
    flayed = false;
}

bool Hand::full(uint8_t cardType) const {
    // If default, we just return empty
    if (cardType == 0)
        return cards.empty();
    // Otherwise we make sure that we aren't at the max hand size
    if (cards.size() < blackjack::maxHandSize)
        return false;
    // Otherwise we make sure the stack size is not full.
    uint8_t stackNum = 0;
    // Iterate through and see how many we have.
    for (auto& i: cards)
        if (i->get_value() == cardType)
            stackNum++;
    // Return whether we're under the stack limit or not.
    return (stackNum > 0 && stackNum < blackjack::maxStackSize);
}

bool Hand::empty() const {
    return cards.empty();
}

int Hand::size() const {
    return cards.size();
}

// Adds one card to the deck and sends it on its merry way
bool Hand::add_card(Card* c) {
    if (cards.size() <= maxHandSize && c != nullptr) {
        cards.push_back(c);
        std::sort(cards.begin(), cards.end(), [](Card* c1, Card* c2) {
            if (c1->get_value() == c2->get_value())
                return c1->get_suit() < c2->get_suit();
            return c1->get_value() < c2->get_value();
        });
    }
    else
        return false;
    return true;
}

Card* Hand::pop_card(int cardNum) {
    if (cardNum < cards.size()) {
        Card* ret = cards[cardNum];
        cards.erase(cards.begin()+cardNum);
        return ret;
    }
    return nullptr;
}


bool Hand::has_normal_cards() {
    return std::any_of(cards.begin(), cards.end(), [this](Card* c)
        { return c->get_suit() != BJ_Suit::SPECIAL; }
    );
}

Card* Hand::operator[](int index) const {
    if (index < cards.size() && index >= 0)
        return cards[index];
    return nullptr;
}

int Hand::get_index(Card* c) {
    for (int i = 0; i < cards.size(); i++) {
        if (c == cards[i])
            return i;
    }
    return -1;
}

std::vector<Card*>& Hand::gather_objects() {
    return cards;
}