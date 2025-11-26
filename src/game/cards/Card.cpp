#include "game/cards/Card.hpp"

uint8_t Card::to_state() {
    // Modifier for calculating the state
    char mod = 0;
    // Spades are the default suit
    if (suit == 'h') mod = 1;
    else if (suit == 'd') mod = 2;
    else if (suit == 'c') mod = 3;
    // Calculate the sprite value
    int ret = value + (mod * 13) - 2;
    // If we're an ace, we add 13
    if (value == 1) ret += 13;
    // Return!
    return ret;
}

Card::Card(int val, char suite)
    : value(abs(val)), suit(suite)
{
    if (value > 13) value = 13;
    t = defaultCardTransform;
    fs = defaultCardFrameState;
    fs.state = to_state();
}

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

void Card::flip_down() {
    flipped = true;
    fs.state = CARD_FLIPPED;
}

bool Card::is_flipped() const {
    return flipped;
}

int Card::get_score() const {
    if (value > 13) return 10;
    return value;
}
