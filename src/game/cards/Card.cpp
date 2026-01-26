#include "../../../include/game/cards/single-cards/Card.hpp"
#include "game/cards/single-cards/Card-Defaults.hpp"

using namespace card;

// mandatory constructor
Card::Card(gan::FrameTable& frame_table, uint32_t value, Suit suit)
    : Sprite(frame_table), value(value), suit(suit), flipped(false)
{
    // specify our default transform
    t = default_card_transform;
    // set it as draggable
    set_draggable();
    // force our animation number
    anim.set_animation(get_anim_num());
    // allows us to assign a user number
    user_tag = 'c';
}

// return the animation number of the card
uint16_t Card::get_anim_num() {
    // If it's a special suit, we return the animation number immediately
    if (suit == SPECIAL)
        return value;
    // Otherwise, we use the standard set of cards
    char mod = 0;
    // Hearts are the default suit so we do nothing there
    if (suit == DIAMOND) mod = 1;
    else if (suit == CLUB) mod = 2;
    else if (suit == SPADE) mod = 3;
    // then we calculate the necessary animation number. (-1 because we move back by -1)
    return value + (mod * 13) - 1;
}

void Card::normalize() {
    // if value is out of range
    if (suit != SPECIAL && value > 13)
        value = 0;
    if (flipped)
        anim.set_animation(flipped_card_anim);
    else
        anim.set_animation(get_anim_num());
}

// flips a card up
void Card::flip_up() {
    flipped = false;
    normalize();
}

// flips a card down
void Card::flip_down() {
    flipped = true;
    normalize();
}

// flips a card generally
void Card::flip () {
    flipped = !flipped;
    normalize();
}

// return true if the card is flipepd down
bool Card::is_flipped() const {
    return flipped;
}

// returns true if the card is a special card
bool Card::is_special() const {
    return suit == SPECIAL;
}

// returns the suit of the card
Suit Card::get_suit() const {
    return suit;
}

// Sets the value of the card
bool Card::set_value(int val) {
    value = val;
    normalize();
}

// increments the value of the card by the value
bool Card::incr_value_by(int val) {
    value += val;
    normalize();
}

long Card::get_score() {
    // create a running score
    long running_score = value;
    // if the score is greater than 13
    if (running_score > 13)
        running_score = 10;
    // apply all the affixes
    for (auto& a : affixes)
        a.modify_score(running_score);
    // return the running score.
    return running_score;
}
