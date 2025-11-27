#include "game/board/Board.hpp"

#include <iostream>

#include "input/InputConstants.hpp"

Board::Board(Deal* player) : player(player) {

    opponent = new Deal(Vertex(3*scene::width/8, scene::height/8, 0));
}

Board::~Board() {
    delete opponent;
}

std::vector<Card *> &Board::get_deck() {
    return deck.get_deck();
}

Deck * Board::get_deck_obj() {
    return &deck;
}

Card* Board::pop_deck_card() {
    return deck.pop_card();
}

void Board::add_deck_card(Card *c) {
    deck.add_card(c);
}

void Board::add_card(Card *c) {
    deck.add_card(c);
}

void Board::add_deck_cards(std::vector<Card *> &cards) {
    deck.add_cards(cards);
}

bool Board::hit_player() {
    Card* popper = deck.pop_card();
    if (popper == nullptr)
        return false;
    player->add_card(popper, false);
    return true;
}

bool Board::hit_player(Card *c) {
    player->add_card(c, 0.6);
    return true;
}

bool Board::hit_opponent() {
    Card* popper = deck.pop_card();
    if (popper == nullptr)
        return false;
    player->add_card(popper, false);
    return true;
}

bool Board::hit_opponent(Card* c) {
    opponent->add_card(c);
    return true;
}

void Board::retrieve_cards() {
    deck.add_cards(player->pop_cards());
    deck.add_cards(opponent->pop_cards());
}

void Board::comp_scores() {
    int pscore = player->get_score();
    int oscore = opponent->get_score();
    if (pscore > targetScore)
        retrieve_cards();
    else if (oscore > targetScore)
        retrieve_cards();
}
