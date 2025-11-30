// ReSharper disable CppTooWideScopeInitStatement
#include "game/GameMaster.hpp"
#include "include/EngineSource.hpp"

// Initializes all the objects needed for the game.
void GameMaster::initialize() {
    // Send everything to the engine
    std::vector<gengine::Object*> allObjects = player.gather_objects();
    bob.add(allObjects);
}

void GameMaster::update() {

}

// Deletes the objects it's responsible for.
GameMaster::~GameMaster() {
    bob.remove(player.gather_objects());
}

// Adds cards to a hand:
void GameMaster::add_card_to_hand(blackjack::Card c) {
    auto card = new blackjack::Card(c);
    if (player.add_card_to_hand(card))
        bob.add(static_cast<gengine::Object*>(card));
    else
        delete card;
}

// Set hand has target
void GameMaster::set_hand_as_target() {
    bob.set_input_target(&player);
}

void GameMaster::blackjack() {
    // If someone busts, then we return the cards to the deck.

}