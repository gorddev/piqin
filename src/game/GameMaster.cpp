// ReSharper disable CppTooWideScopeInitStatement
#include "game/GameMaster.hpp"
#include "include/EngineSource.hpp"

// Initializes all the objects needed for the game.
void GameMaster::initialize() {
    // Send everything to the engine
    std::vector<gengine::Object*> allObjects = board.gather_objects();
    bob.add_objects(allObjects);
    round = new blackjack::Round(&board);
    round->begin();
}

void GameMaster::update() {
    if (round->done) {
        delete round;
        round = new blackjack::Round(&board);
        bob.set_input_target(round);
        round->begin();
    }
    blackjack::pather.update(board.deck);
}

// Deletes the objects it's responsible for.
GameMaster::~GameMaster() {
    bob.remove_objects(board.gather_objects());
}

// Adds cards to a hand:
void GameMaster::add_card_to_hand(blackjack::Card c) {
    auto card = new blackjack::Card(c);
    if (board.add_card_to_hand(card))
        bob.add_object(card);
    else
        delete card;
}

// Set hand has target
void GameMaster::set_hand_as_target() {
    bob.set_input_target(round);
}

void GameMaster::blackjack() {
    // If someone busts, then we return the cards to the deck.

}