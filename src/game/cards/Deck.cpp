#include "game/cards/Deck.hpp"

#include <iostream>


static const Transform default_draw_deck = {
    Vertex(7*scene::width/8, 3*scene::height/8, 0),
    53,
    37,
    1,
    DECK_Z_BASE
};

Deck::Deck() {
    // Sets up the parameters of our default deck
    t = default_draw_deck;
    cardsOnDeck = 0;
    // Gets the sprite we render with.
    fs.sheet_id = ASSET_DRAW_DECK_ID;
    fs.state=2;

    for (int i = 1; i <= 13; i++) {
        initialize_card(new Card(i, 'h'));
        initialize_card(new Card(i, 'c'));
        initialize_card(new Card(i, 'd'));
        initialize_card(new Card(i, 's'));
    }
    // Now we can make the deck!
}

Path Deck::get_path(Vertex pos) const {
    Vertex target = t.pos + Vertex(deck_height(), deck_height()+4, 0);

    Path p = {
        target,
        pos,
        PATH_LINEAR,
        0.5
    };
    return p;
}

void Deck::card_arrival() {
    fs.state = 2 - ((STANDARD_DECK_SIZE-1)/++cardsOnDeck);
}

void Deck::card_removal() {
    fs.state = 2 - ((STANDARD_DECK_SIZE-1)/--cardsOnDeck);
}

short Deck::deck_height() const {
    if (cardsOnDeck==0) return 0;
    return ((STANDARD_DECK_SIZE-1)/cardsOnDeck); // NOLINT(*-narrowing-conversions)
}

void Deck::initialize_card(Card *c) {

    deck.push_back(c);

    c->set_z_index(DECK_Z_BASE + deck.size()/MAX_DECK_SIZE);

    c->flip_down();
    c->shadow = false;
    c->hidden = true;
    // We say this card is immediately arrived on the deck
    card_arrival();
}


void Deck::add_card(Card* c) {
    deck.push_back(c);
    // This card is not immediately on the deck, so it's traveling.
    traveling.push_back(c);
    c->shadow = false;
    c->set_z_index(DECK_Z_BASE + deck.size()/MAX_DECK_SIZE);
    c->flip_down();
    c->set_path(get_path(c->pos()));

}

void Deck::add_cards(std::vector<Card*> cards) {
    for (auto c : cards)
        add_card(c);
}

Card* Deck::pop_card() {
    if (deck.size() == 0) return nullptr;
    int cardNum = random()% deck.size();
    Card* c = deck[cardNum];
    deck.erase(deck.begin()+cardNum);
    c->set_pos(t.pos);
    c->hidden = false;
    c->shadow = true;
    card_removal();
    return c;
}

void Deck::update_travel_cards(){
    for (int i = 0; i < traveling.size(); i++) {
        if (traveling[i]->get_path() == nullptr) {
            std::cerr<<"no path\n";
            traveling[i]->hidden = true;
            traveling[i]->shadow = false;
            card_arrival();
            std::swap(traveling[i], traveling.back());
            traveling.pop_back();
            i--;
        }
    }
}

std::vector<Card *> & Deck::get_deck() {
    return deck;
}


