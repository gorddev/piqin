// ReSharper disable CppTooWideScopeInitStatement
#include "game/GameMaster.hpp"

#include <iostream>
#include <ostream>

#include "Engine.hpp"

// Initializes all the objects needed for the game.
void GameMaster::initialize() {
    game::hand = new Hand();
    engine::om->add_object(game::hand->get_pointer());
}

void GameMaster::update() {
    game::hand->update_cards();
}

// Deletes the objects it's responsible for.
GameMaster::~GameMaster() {
    delete game::hand;
}

// Adds cards to a hand:

void GameMaster::add_card_to_hand(Card c) {
    if (game::hand == nullptr) return;
    auto card = new Card(c);
    if (game::hand->add_card(card))
        engine::om->add_object(card);
    else
        delete card;
}

void GameMaster::set_hand_as_target() {
    engine::input->setInputTarget(game::hand);
}
