#include "game/blackjack/cards/Deck.hpp"
#include "game/blackjack/BjConstants.hpp"
#include <iostream>

// constructor
Deck::Deck() : discard(1, 's') {
    // Sets up the parameters of our default deck
    t = default_deck;
    cardsOnDeck = 0;
    // Gets the sprite we render with.
    fs.sheet_id = ASSET_CARD_STACK_ID;
    fs.state = 2;

    for (int i = 1; i <= 13; i++) {
        initialize_card(new Card(i, 'h'));
        initialize_card(new Card(i, 'c'));
        initialize_card(new Card(i, 'd'));
        initialize_card(new Card(i, 's'));
    }

    discard.hidden = true;
    std::cerr << discard.pos().to_string() << std::endl;
    discard.set_pos(BJ_DEFAULT_DISCARD_POS);
    std::cerr << BJ_DEFAULT_DISCARD_POS.to_string() << std::endl;
    std::cerr << discard.pos().to_string() << std::endl;
}

// destructor
Deck::~Deck() {
    for (auto& c : drawPile) delete c;
    for (auto& c : discardPile) delete c;
}

// get_path_deck
Path Deck::get_path_deck(Vertex pos) const {
    Vertex target = t.pos + Vertex(deck_height(), deck_height()+4, DECK_Z_BASE + drawPile.size()/MAX_DECK_SIZE);

    Path p = {
        target,
        pos,
        PATH_SINE,
        0.25
    };
    return p;
}


void Deck::card_arrival(Card*c) {
    hidden = false;
    fs.state =  5.0f*(STANDARD_DECK_SIZE - (++cardsOnDeck-1))/52.0f;
    c->hidden = true;
    c->shadow = false;
    c->flip_down();
}

void Deck::card_removal(Card*c, int i) {
    hidden = false;
    if (cardsOnDeck > 1)
        fs.state = 5.0f*(STANDARD_DECK_SIZE - (--cardsOnDeck - 1))/52.0f;
    else {
        hidden = true;
        cardsOnDeck--;
    }
    drawPile.erase(drawPile.begin()+i);
    c->set_pos(t.pos);
    c->hidden = false;
    c->shadow = true;
}

short Deck::deck_height() const {
    if (cardsOnDeck==0) return 0;
    return ((STANDARD_DECK_SIZE-1)/cardsOnDeck); // NOLINT(*-narrowing-conversions)
}

void Deck::initialize_card(Card *c) {

    drawPile.push_back(c);
    c->set_z(DECK_Z_BASE + drawPile.size()/MAX_DECK_SIZE);

    // We say this card is immediately arrived on the deck
    card_arrival(c);
}


void Deck::add_card(Card* c) {
    drawPile.push_back(c);
    // This card is not immediately on the deck, so it's traveling.
    traveling.push_back(c);
    c->shadow = false;
    c->flip_down();
    c->set_path(get_path_deck(c->pos()));

}

void Deck::add_cards(std::vector<Card*> cards) {
    for (auto c : cards)
        add_card(c);
}

Card* Deck::pop_card() {
    if (drawPile.empty() || cardsOnDeck == 0) return nullptr;
    int cardNum = random()% drawPile.size();
    Card* c = drawPile[cardNum];
    card_removal(c, cardNum);
    c->flip_up();
    if (drawPile.empty())
        pop_discard_pile();

    return c;
}

void Deck::update_travel_cards(){
    for (int i = 0; i < traveling.size(); i++) {
        if (traveling[i]->get_path() == nullptr) {
            card_arrival(traveling[i]);
            std::swap(traveling[i], traveling.back());
            traveling.pop_back();
            i--;
        }
    }
}

void Deck::discard_card(Card *c, int cardNum) {
    discardPile.push_back(c);
    Vertex unit;
    unit.randomize(20);
    c->shadow = false;
    c->set_z(DECK_Z_BASE + ((discardPile.size())/MAX_DECK_SIZE));
    c->set_shake(SHAKE_CIRCULAR, 1.0, (20*cardNum) + 100 + random()%300, 3, false);
    std::cerr << discard.pos().to_string() << std::endl;
    c->set_path(discard.pos()+unit, PATH_BALLOON, 0.5);
}

void Deck::discard_cards(std::vector<Card *> cards) {
    for (auto& c : cards)
        discard_card(c);
}

void Deck::refresh_discard_pile() {
    for (auto& c : discardPile) {
        Vertex unit;
        unit.randomize(20);
        c->shadow = false;
        c->set_z(DECK_Z_BASE + ((discardPile.size())/MAX_DECK_SIZE));
        c->set_path(discard.pos() + unit, PATH_BALLOON, 0.5);
    }
}


void Deck::pop_discard_pile() {
    add_cards(discardPile);
    discardPile.clear();
}

Card* Deck::pop_discard_card(int num) {
    if (num >= discardPile.size())
        return nullptr;
    Card* c = discardPile[num];
    discardPile.erase(discardPile.begin()+num);
    return c;
}

std::vector<Card *> & Deck::gather_objects() {
    return drawPile;
}


