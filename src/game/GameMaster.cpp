// ReSharper disable CppTooWideScopeInitStatement
#include "game/GameMaster.hpp"

#include <iostream>
#include "Engine.hpp"

// Initializes all the objects needed for the game.
void GameMaster::initialize() {

    player = new BlackjackPlayer();

    // Next, we send all the cards & objects to the object manager
    engine::om->add_objects(player->gather_objects());

    // We must do this to render anything, because the render manager uses the object manager's processing in order to render objects correctly.
}

void GameMaster::update() {

}

// Deletes the objects it's responsible for.
GameMaster::~GameMaster() {
    engine::om->remove_objects(player->gather_objects());
    delete player;
}

// Adds cards to a hand:
void GameMaster::add_card_to_hand(Card c) {
    auto card = new Card(c);
    if (player->add_card_to_hand(card))
        engine::om->add_object(card);
    else
        delete card;
    std::cerr << card->state() + 0 << std::endl;
}

// Set hand has target
void GameMaster::set_hand_as_target() {
    engine::input->setInputTarget(player);
}

void GameMaster::blackjack() {
    // If someone busts, then we return the cards to the deck.

}