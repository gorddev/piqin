#include "game/blackjack/bj-objects/Deck.hpp"
#include "game/blackjack/BJEnums.hpp"
#include "EngineSource.hpp"
#include <random>

using namespace blackjack;

// constructor
Deck::Deck() {
    // Sets up the parameters of our default deck
    t = default_deck;
    // Gets the sprite we render with.
    fs.sheet_id = ASSET_CARD_STACK_ID;
    fs.state = 2;

    for (int i = 1; i <= 12; i++) {
        add_card(new Card(i, BJ_Suit::HEART));
        add_card(new Card(i, BJ_Suit::CLUB));
        add_card(new Card(i, BJ_Suit::DIAMOND));
        add_card(new Card(i, BJ_Suit::SPADE));
    }
}

// destructor
Deck::~Deck() {
    for (auto& c : drawPile) delete c;
}

void Deck::update_state() {
    fs.state = static_cast<uint8_t>(5.0f*(STANDARD_DECK_SIZE - ((drawPile.size())-1.0f))/52.0f);
}

void Deck::prep_card_for_pop(Card*c) {
    if (drawPile.size() > 1)
        fs.state = 5.0f*(STANDARD_DECK_SIZE - (drawPile.size()))/52.0f;
    else
        hidden = true;
    c->set_pos(t.pos);
    c->hidden = false;
    c->shadow = true;
}

short Deck::stack_height() const {
    return 6-((STANDARD_DECK_SIZE-1)/drawPile.size()); // NOLINT(*-narrowing-conversions)
}

void Deck::add_card(Card* c) {
    hidden = false;
    drawPile.push_back(c);
    c->set_z(DECK_Z_BASE + drawPile.size()/MAX_DECK_SIZE);
    c->hidden = true;
    c->flip_down();
}

Card* Deck::pop_card() {
    if (drawPile.empty()) return nullptr;
    // Random card!
    int i = gengine::GENG_Rand.rint() % drawPile.size(); // NOLINT(*-narrowing-conversions)
    Card* c = drawPile[i];
    drawPile.erase(drawPile.begin() + i);
    prep_card_for_pop(c);
    return c;
}

std::vector<Card *> Deck::pop_all() {
    std::vector<Card*> temp = drawPile;
    for (auto& c : drawPile)
        prep_card_for_pop(c);
    drawPile.clear();
    return temp;
}

std::vector<Card *> & Deck::gather_objects() {
    return drawPile;
}

bool Deck::empty() {
    return drawPile.empty();
}

int Deck::size() {
    return drawPile.size();
}