#include "../../../../include/game/blackjack/bj-objects/Draw.hpp"

#include <iostream>

#include "game/blackjack/BJEnums.hpp"

using namespace blackjack;

Draw::Draw(gengine::Vertex pos) : pos(pos) {}

int Draw::get_num_cards() {
    return draw.size();
}

// Adds a card and assigns it a gengine::Path.
void Draw::add_card(Card *card, bool flipped) {
    card->flip_up();
    // Lets us flip it down if we want.
    if (flipped)
        card->flip_down();
    short posi = draw.size();
    // This makes sure we put cards in spots previous cards didnt exist
    if (!removals.empty()) {
        std::sort(removals.begin(), removals.end(), [](short a, short b) {return a > b;});
        posi = removals.back();
        removals.pop_back();
        draw.insert(draw.begin() + (posi - 1), card);
    }
    else {
        draw.push_back(card);
    }
}

// Gives all the cards back to the Dealer
std::vector<Card*> Draw::pop_cards() {
    std::vector<Card*> cards = draw;
    draw.clear();
    removals.clear();
    return cards;
}

Card * Draw::pop_card(int cardNum) {
    if (draw.empty())
        return nullptr;
    removals.push_back(cardNum+1);
    if (cardNum < 0 || cardNum >= draw.size())
        cardNum = draw.size() - 1;
    Card* c = draw[cardNum];
    draw.erase(draw.begin() + cardNum);
    if (draw.empty())
        removals.clear();
    return c;
}

int Draw::get_score() const {
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

bool Draw::will_bust(Card *c) {
    return (c->get_score() + get_score() > blackjack::roundScore);
}

int Draw::size() const {
    return draw.size();
}

bool Draw::empty() const {
    return draw.empty();
}

void Draw::set_pos(gengine::Vertex v) {
    pos = v;
}

Card * Draw::operator[](int index) {
    if (index < 0 || index >= draw.size())
        return nullptr;
    return draw[index];
}

std::vector<Card*>& Draw::gather_objects() {
    return draw;
}

gengine::Vertex Draw::get_pos() {
    return pos;
}

int Draw::get_index(Card* c) {
    for (int i = 0; i < draw.size(); i++) {
        if (c == draw[i])
            return i;
    }
    return -1;
}

void Draw::print_ptrs() const {
    std::cerr << "Printing ptrs (" << gengine::glb::scene.time << ")\n";
    for (int i = 0; i < draw.size(); i++) {
        std::cerr << "c[" << i  << "] = " << draw[i] << std::endl;
    }
}