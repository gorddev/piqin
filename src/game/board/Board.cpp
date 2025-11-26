#include "game/board/Board.hpp"

#include <iostream>

#include "input/InputConstants.hpp"

Board::Board() {
    deckPos = Vertex(7*scene::width/8, 3*scene::height/8, 0);
    for (int i = 1; i <= 13; i++) {
        add_card(new Card(i, 'h'));
        add_card(new Card(i, 'c'));
        add_card(new Card(i, 'd'));
        add_card(new Card(i, 's'));
    }
}


std::vector<Card *> &Board::get_deck() {
    return deck;
}

Path Board::get_card_path(Card*& c) const {
    Vertex target(deckPos.x,
        deckPos.y,
        0);
    Path p = {
        target,
        c->pos(),
        PATH_BALLOON,
        1.0
    };
    return p;
}

bool Board::remove_card(Card *c) {
    for (int i = 0; i < deck.size(); i++) {
        if (deck[i] == c) {
            deck.erase(deck.begin() + i);
            return true;
        }
    }
    return false;
}

Card* Board::pop_card(const int i) {
    if (i < deck.size()) {
        Card* c = deck[i];
        c->shadow = true;
        deck.erase(deck.begin() + i);
        return c;
    }
    return nullptr;
}

void Board::add_card(Card *c) {
    deck.push_back(c);
    c->set_z_index(BOARD_Z_BASE + deck.size()/MAX_DECK_SIZE);
    c->shadow = false;
    c->flip_down();
    c->set_path(get_card_path(c));
}

bool Board::hit_player() {
    std::cerr << "hit?\n";
    if (deck.size() <= 0) return false;
    int cardnum = random() % deck.size();
    player.add_card(pop_card(cardnum), false);
    return true;
}

bool Board::hit_player(Card *c) {
    player.add_card(c, 0.6);
    return true;
}

bool Board::hit_opponent() {
    std::cerr << "opponent?\n";
    if (deck.size() <= 0) return false;
    int cardnum = random() % deck.size();
    opponent.add_card(pop_card(cardnum), opponent.get_num_cards() == 0);
    return true;
}

bool Board::hit_opponent(Card* c) {
    opponent.add_card(c);
    return true;
}

void Board::retrieve_cards() {
    for (auto& i : player.pop_cards()) {
        deck.push_back(i);
        i->set_path(get_card_path(i));
        i->shadow = false;
        i->flip_down();
    }
    for (auto& i : opponent.pop_cards()) {
        deck.push_back(i);
        i->set_path(get_card_path(i));
        i->shadow = false;
        i->flip_down();
    }
}

void Board::comp_scores() {
    int pscore = player.get_score();
    int oscore = opponent.get_score();
    if (pscore > targetScore)
        retrieve_cards();
    else if (oscore > targetScore)
        retrieve_cards();
}

bool Board::getPress(short keybind) {
    switch_to_hand = (keybind == INPUT_MODIFY);
    if (keybind == INPUT_SELECT)
        return hit_player();
    if (keybind == INPUT_BACK)
        return hit_opponent();
    return false;
}
