#include <game/board/Deck.hpp>

using namespace card;

Deck::Deck(gan::FrameTable& card_table) {
    cards.reserve(52);
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 4; j++) {
            cards.push_back(new Card(card_table, i+1, static_cast<Suit>(j)));
            cards.back()->t.pos = {i*32.f + 4.f, j*32.f +4.f};
        }
    }
}

Card* Deck::pop_card() {
    // grab random card
    int card_index = random() % cards.size();
    // get the card into an object
    Card* ret = cards[card_index];
    // flip the card up
    ret->flip_up();
    // swap the card with the last
    std::swap(cards[card_index], cards.back());
    // pop the back
    cards.pop_back();
    // return it
    return ret;
}

void Deck::add_card(Card* card) {
    // adds a card to the deck
    cards.push_back(card);
    // flips the card down
    card->flip_down();
}

// //////////////////////
// Private Function
// //////////////////
std::vector<Card*>& Deck::gather_objects() {
    return cards;
}
