#include "game/board/Deal.hpp"

Deal::Deal() : pos(scene::width/2, scene::height/2, 0) {}
Deal::Deal(Vertex pos) : pos(pos) {}

int Deal::get_num_cards() {
    return deal.size();
}

Path Deal::get_deal_path(Card*& c) {
    const Vertex target(pos.x - scene::width/8 + deal.size()*12,
        pos.y, pos.z);
    Path p = {
        target,
        c->pos(),
        PATH_TORPEDO,
        1.0
    };
    return p;
}

// Adds a card and assigns it a path.
void Deal::add_card(Card *card, bool flipped) {
    if (flipped & !card->is_flipped())
        card->flip();
    card->set_z_index(DEAL_Z_BASE + (deal.size()/100.f));
    deal.push_back(card);
    card->set_path(get_deal_path(card));
}

// Gives all the cards back to the Dealer
std::vector<Card*> Deal::pop_cards() {
    std::vector<Card*> cards = deal;
    deal.clear();
    return cards;
}

int Deal::get_score(int target) {
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