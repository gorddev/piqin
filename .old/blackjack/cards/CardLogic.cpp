#include "../../../../include/engine/debug/geng_debug.hpp"
#include "game/blackjack/cards/Card.hpp"

using namespace blackjack;

// Adjust the value of a card by a certain amount
bool Card::adjust_value(int num) {
    // Can't change the value of special cards.
    if (suit == BJ_Suit::SPECIAL) return false;
    // Otherwise continue on as normal
    value = (value + (num%14));
    if (value <= 0) value = 13 + value;
    if (value == 0) value++;
    // Make sure to update the state
    anim.set_animation(to_anim_num());
    return true;
}

// Gets the score of a card.
int Card::get_score() const {
    auto addr = reinterpret_cast<uintptr_t>(this);
    size_t align = alignof(Card);
    if (addr % align != 0) {
        // THis is what happens if we feed card that's not a card :o
        glog::err.src("Card::get_score()") << "card::get_score() alignment is not 0" << glog::endlog;
    }

    if (suit == BJ_Suit::SPECIAL) return 0;
    if (value > 10) return 10;
    return value;
    // If it's a special card, return 0
    if (suit == BJ_Suit::SPECIAL) return 0;
    if (value > 10) return 10;
    return value;
}

BJ_Suit Card::get_suit() const {
    return suit;
}

