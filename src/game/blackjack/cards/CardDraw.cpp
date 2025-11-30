#include "../../../../include/game/blackjack/cards/CardDraw.hpp"

#include <iostream>

#include "game/blackjack/BJEnums.hpp"

using namespace blackjack;

CardDraw::CardDraw(gengine::Vertex pos) : pos(pos) {}

int CardDraw::get_num_cards() {
    return draw.size();
}

gengine::Path CardDraw::get_draw_path(Card *&c, short index) {

    gengine::Vertex target(pos.x - gengine::glb::scene.width/8.0f + index*24.0f, // NOLINT(*-narrowing-conversions)
        pos.y - random()%8, DRAW_Z_BASE + (index/10.f)); // NOLINT(*-narrowing-conversions)
    gengine::Path p = {
        target,
        c->pos(),
        gengine::GENG_Path::BALLOON,
        0.5
    };
    std::cerr << "target.z: " << target.z << std::endl;
    return p;
}

// Adds a card and assigns it a gengine::Path.
void CardDraw::add_card(Card *card, bool flipped) {
    card->flip_up();
    // Lets us flip it down if we want.
    if (flipped)
        card->flip_down();
    short posi = draw.size();
    if (!removals.empty()) {
        std::sort(removals.begin(), removals.end(), [](short a, short b) {return a > b;});
        posi = removals.back();
        removals.pop_back();
        draw.insert(draw.begin() + (posi - 1), card);
    }
    else
        draw.push_back(card);
    card->set_path(get_draw_path(card, posi));
}

// Gives all the cards back to the Dealer
std::vector<Card*> CardDraw::pop_cards() {
    std::vector<Card*> cards = draw;
    draw.clear();
    removals.clear();
    return cards;
}

Card * CardDraw::pop_card(int cardNum) {
    if (draw.size() == 0)
        return nullptr;
    removals.push_back(cardNum+1);
    if (cardNum < 0 || cardNum >= draw.size())
        cardNum = draw.size() - 1;
    Card* c = draw[cardNum];
    draw.erase(draw.begin() + cardNum);
    if (draw.size() == 0)
        removals.clear();
    return c;
}

int CardDraw::get_score() const {
    // Calculates the score of our hand.
    int sum = 0;
    int numAces = 0;
    for (auto& card : draw) {
        int score = card->get_score();
        sum+=score;
        if (score==1)
            numAces++;
    }
    for (int i = numAces; (sum <= blackjack::roundScore - 10) && i > 0; i--)
        sum+=10;
    return sum;
}

bool CardDraw::will_bust(Card *c) {
    return (c->get_score() + get_score() > blackjack::roundScore);
}

std::vector<Card*>& CardDraw::card_list() {
    return draw;
}

int CardDraw::size() const {
    return draw.size();
}

bool CardDraw::empty() const {
    return draw.empty();
}

void CardDraw::set_pos(gengine::Vertex v) {
    pos = v;
}

Card * CardDraw::operator[](int index) {
    if (index < 0 || index >= draw.size())
        return nullptr;
    return draw[index];
}
