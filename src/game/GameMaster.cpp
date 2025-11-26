// ReSharper disable CppTooWideScopeInitStatement
#include "game/GameMaster.hpp"

#include <iostream>

#include "Engine.hpp"
#include "utilities/basisu_transcoder/basisu_containers.h"

// Initializes all the objects needed for the game.
void GameMaster::initialize() {
    hand = new Hand();
    board = new Board();
    engine::om->add_object(hand->get_selector());
    std::vector<Object*> send(board->get_deck().size());
    for (int i = 0; i < send.size(); i++) {
        send[i] = board->get_deck()[i];
    }
    engine::om->add_objects(send);
}

void GameMaster::update() {
    hand->update_cards();
}

// Deletes the objects it's responsible for.
GameMaster::~GameMaster() {
    delete hand;
}

// Adds cards to a hand:
void GameMaster::add_card_to_hand(Card c) {
    if (hand == nullptr) return;
    auto card = new Card(c);
    if (hand->add_card(card))
        engine::om->add_object(card);
    else
        delete card;
}

// Set hand has target
void GameMaster::set_hand_as_target() {
    engine::input->setInputTarget(hand);
}

void GameMaster::blackjack() {
    if (board->switch_to_hand)
        engine::input->setInputTarget(hand);
    else {
        engine::input->setInputTarget(board);
    }
}