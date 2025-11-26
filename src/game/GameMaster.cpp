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
    // Handes the switching of inputs
    // If our board tells us to switch to the hand, we switch
    if (hand->get_num_cards() == 0)
        hand -> flayed = false;
    if (board->switch_to_hand) {
        engine::input->setInputTarget(hand);
        hand->heldKeys[INPUT_MODIFY] = true;
        board->switch_to_hand = false;
    }
    // If our hand tells us to switch to the board, we switch.
    else if (!hand->flayed) {
        engine::input->setInputTarget(board);
    }

    if (hand->popcard) {
        board->hit_player(hand->pop_card());
        hand->popcard = false;
    }

    board->comp_scores();
    // If our hand tells us to pop a card
}