#include "game/blackjack/Hand.hpp"

#include <iostream>

#include "ObjectManager.hpp"
#include "game/blackjack/BjConstants.hpp"

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
    if (cards.size() <= blackjack::maxHandSize && c != nullptr) {
        cards.push_back(c);
        std::sort(cards.begin(), cards.end(), [](Card* c1, Card* c2) {
            if (c1->get_value() == c2->get_value())
                return c1->get_suit() < c2->get_suit();
            return c1->get_value() < c2->get_value();
        });
        for (int i = 0; i < cards.size(); i++)
            cards[i]->set_z_index(HAND_Z_BASE+i/(blackjack::maxHandSize*2.0f)); // NOLINT(*-narrowing-conversions)

        update_cards();
    }
    else
        return false;
    return true;
}

Card* Hand::pop_card(int cardNum) {
    if (cardNum < cards.size()) {
        Card* ret = cards[cardNum];
        cards.erase(cards.begin()+cardNum);
        for (int i = cardNum; i < cards.size(); i++) {
            cards[i]->set_z_index(HAND_Z_BASE+i/(blackjack::maxHandSize*2.0f));
        }
        update_cards();
        return ret;
    }
    return nullptr;
}


Path Hand::get_card_path(int index, int& center, int& height, float speed, Vertex mod) const {
    int flay = (flayed) ? (20) : 0;
    char target = (index == lastTarget);
    Path p = {
        {
            // Space each card more x-wise if hand is flayed
            center + (14+flay) * (index - ((cards.size()-1)/2.0f)) + mod[0], // NOLINT(*-narrowing-conversions)
            // Set default height. If not flayed, stagger each card vertically from the last picked.
            (height - (flay*1.25f))
                + ((!flayed) * abs(lastTarget-index)*2)
                - (flayed) + mod[1], // NOLINT(*-narrowing-conversions)
            HAND_Z_BASE + index/(blackjack::maxHandSize*2.0f) + mod[2] // NOLINT(*-narrowing-conversions)
        },
        cards[index]->pos(),
        PATH_BALLOON,
        speed
    };
    return p;
}

void Hand::update_cards() {
    int center = scene::width/2;
    int height = 3*scene::height/4;

    for (int i = 0; i < cards.size(); i++)
        cards[i]->set_path(get_card_path(i, center, height));

    Vertex offset = (flayed) ? Vertex(0,0,0) : Vertex(0,10,0);
}

bool Hand::has_normal_cards() {
    for (auto& c: cards) {
        if (c->get_suit() != SPECIAL_CARD_SUIT)
            return true;
    }
    return false;
}

Card* Hand::operator[](int index) const {
    if (index < cards.size() && index >= 0)
        return cards[index];
    return nullptr;
}
