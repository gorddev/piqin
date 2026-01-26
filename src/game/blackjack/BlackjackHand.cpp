#include "game/blackjack/BlackjackHand.hpp"

using namespace bj;

BlackjackHand::BlackjackHand(gan::pos2 pos)
    : position(pos) {}

card::Card * BlackjackHand::pop_card(card::Card *card) {
    for (auto& c: cards) {
        if (c == card) {
            std::swap(c, cards.back());
            cards.pop_back();
            return card;
        }
    }
    return nullptr;
}

card::Card* BlackjackHand::pop_card(uint_fast16_t index) {
    if (index >= cards.size())
        return nullptr;
    auto card = cards[index];
    std::swap(cards[index], cards.back());
    cards.pop_back();
    return card;
}

void BlackjackHand::add_card(card::Card *card) {
    if (std::find(cards.begin(), cards.end(), card) != cards.end()) {
        cards.push_back(card);

    }
}

long BlackjackHand::get_score() {
    long running_score = 0;
    for (auto& c: cards) {
        running_score += c->get_score();
    }
    return running_score;
}



