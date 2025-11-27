// ReSharper disable CppTooWideScopeInitStatement
#include "game/GameMaster.hpp"

#include <iostream>

#include "Engine.hpp"
#include "utilities/basisu_transcoder/basisu_containers.h"

// Initializes all the objects needed for the game.
void GameMaster::initialize() {
    // First we initialize the player
    hand = new PlayerHand();
    // Then we initialize the board and give the board access to the player's deal
    board = new Board(hand->get_deal());

    // Then we add the hand's selector to the object manager
    engine::om->add_object(hand->get_hand_selector());
    // And we add the deck to the hand's selector
    engine::om->add_object(board->get_deck_obj());

    // Next, we send all the cards in the deck to the object manager.
    std::vector<Object*> send(board->get_deck().size());
    // We need to convert Card* -> Object*
    for (int i = 0; i < send.size(); i++) {
        send[i] = board->get_deck()[i];
    }
    // And then we send them off!
    engine::om->add_objects(send);

    // We must do this to render anything, because the render manager uses the object manager's processing in order to render objects correctly.
}

void GameMaster::update() {
    hand->update_cards();
    board->get_deck_obj()->update_travel_cards();
}

// Deletes the objects it's responsible for.
GameMaster::~GameMaster() {
    delete hand;
}

// Adds cards to a hand:
void GameMaster::add_card_to_hand(Card c) {
    if (hand == nullptr) return;
    auto card = new Card(c);
    if (hand->add_card_to_hand(card))
        engine::om->add_object(card);
    else
        delete card;
}

// Set hand has target
void GameMaster::set_hand_as_target() {
    engine::input->setInputTarget(hand);
}

void GameMaster::blackjack() {
    // If someone busts, then we return the cards to the deck.
    board->comp_scores();
    // Request hits
    if (hand->hit) {
        board->hit_player();
        hand->hit = false;
    }
    if (hand->stay) {
        board->hit_opponent();
        hand->stay = false;
    }
}