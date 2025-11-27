#include "game/blackjack/cards/Card.hpp"

#include <iostream>

// Gets the state of a card.
uint8_t Card::to_state() {
    // Modifier for calculating the state

    // Special cards
    if (suit == 'x')
        return 52 + value;

    // Otherwise default set of cards
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

// Create a card with a value and suite
Card::Card(int val, char suite)
    : value(abs(val)), suit(suite) {
    if (value > 13) value = 13;
    t = defaultCardTransform;
    fs = defaultCardFrameState;
    fs.state = to_state();
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

int Card::get_score() const {
    // Special cards
    if (suit == 'x') return 0;
    if (value > 13) return 10;
    return value;
}

void Card::increase_value(int num) {
    value = (value + num) % 14;
    if (value == 0) value++;
    fs.state = to_state();
}

void Card::change_into_card(Card* c) {
    value = c->value;
    suit = c->suit;
    fs.state = c->state();
}