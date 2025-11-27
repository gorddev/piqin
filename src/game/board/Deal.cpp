#include "../../../include/game/blackjack/cards/Deal.hpp"

#include <iostream>

Deal::Deal() : pos(scene::width/2, scene::height/2, 0) {}
Deal::Deal(Vertex pos) : pos(pos) {}

int Deal::get_num_cards() {
    return deal.size();
}

Path Deal::get_deal_path(Card *&c, float speed) {
    const Vertex target(pos.x - scene::width/8 + deal.size()*24,
        pos.y - random()%8, pos.z);
    Path p = {
        target,
        c->pos(),
        PATH_BALLOON,
        speed
    };
    return p;
}

// Adds a card and assigns it a path.
void Deal::add_card(Card *card, bool flipped) {
    if (flipped)
        card->flip_down();
    else
        card->flip_up();
    card->set_z_index(DEAL_Z_BASE + (deal.size()/100.f));
    deal.push_back(card);
    card->set_path(get_deal_path(card));
}

void Deal::add_card(Card *card, double speed) {
    card->set_z_index(DEAL_Z_BASE + (deal.size()/100.f));
    deal.push_back(card);
    card->set_path(get_deal_path(card, speed));
}

// Gives all the cards back to the Dealer
std::vector<Card*> Deal::pop_cards() {
    std::vector<Card*> cards = deal;
    deal.clear();
    return cards;
}

Card * Deal::pop_card(int cardNum) {
    if (deal.size() == 0)
        return nullptr;
    if (cardNum < 0 || cardNum >= deal.size())
        cardNum = deal.size() - 1;
    Card* c = deal[cardNum];
    deal.erase(deal.begin() + cardNum);
    return c;
}

int Deal::get_score(const int target) const {
    // Calculates the score of our hand.
    int sum = 0;
    int numAces = 0;
    for (auto& card : deal) {
        int score = card->get_score();
        sum+=score;
        if (score==1)
            numAces++;
    }
    for (int i = numAces; (sum <= target - 10) && i > 0; i--)
        sum+=10;
    return sum;
}
