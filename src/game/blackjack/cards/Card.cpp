#include "game/blackjack/cards/Card.hpp"

#include "game/blackjack/BJEnums.hpp"

/* This file contains all the proper set up for making a card */
/* Game logic for card behavior is found in CardLogic.hpp */

using namespace blackjack;

// Gets the state of a card.
uint8_t Card::to_state() {
    if (suit == BJ_Suit::SPECIAL)
        return value;
    // Otherwise default set of cards
    char mod = 0;
    // Spades are the default suit
    if (suit == BJ_Suit::HEART) mod = 1;
    else if (suit == BJ_Suit::DIAMOND) mod = 2;
    else if (suit == BJ_Suit::CLUB) mod = 3;
    // Calculate the sprite value
    int ret = value + (mod * 13) - 2;
    // If we're an ace, we add 13
    if (value == 1) ret += 13;
    // Return!
    return ret;
}

// Create a card with a value and suite
Card::Card(int val, BJ_Suit suite)
    : value(abs(val)), suit(suite) {

    if (value > 13 && suit != BJ_Suit::SPECIAL) value = 13;
    t = defaultCardTransform;
    fs = defaultCardFrameState;
    fs.state = to_state();
    fs.baseState = to_state();
    hidden = true;
}

// Create a card from another card.
Card::Card(Card& c) {
    value = c.value;
    suit = c.suit;
    flipped = c.flipped;
    t = c.t;
    fs = c.fs;
}

bool Card::operator==(const Card c) const {
    return value == c.value && suit == c.suit;
}

void Card::flip() {
    flipped = !flipped;
    fs.state = (flipped) ? CARD_FLIPPED : to_state();
}

void Card::flip_up() {
    flipped = false;
    fs.state = to_state();
}

void Card::flip_down() {
    flipped = true;
    fs.state = CARD_FLIPPED;
}

bool Card::is_flipped() const {
    return flipped;
}

int Card::get_value() const {
    return value;
}

bool Card::special() const {
    return suit == BJ_Suit::SPECIAL;
}

bool Card::use(Card *c) {

    if (value == BJ_CARD_INCREMENT) {
        if (c->special()) {
            c->set_shake(BJ_SHAKE_DENY);
            return false;
        }
        c->adjust_value(3);
        c->set_shake(gengine::GENG_Shake::CIRCULAR, 3, 400, -2);
    }
    else if (value == BJ_CARD_DECREMENT) {
        if (c->special()) {
            c->set_shake(BJ_SHAKE_DENY);
            return false;
        }
        c->adjust_value(-3);
        c->set_shake(gengine::GENG_Shake::CIRCULAR, 3, 400, 2);
    }
    return true;
}