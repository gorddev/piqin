#include "game/blackjack/Board.hpp"

using namespace blackjack;

Board::Board() :
        playerDraw(BJ_DEFAULT_PLAYER_POS),
        opponentDraw(BJ_DEFAULT_OPPONENT_POS),
        slct(), hand(), discardTemp(1),
        menu(
        {
            &hand.gather_objects(),
            &playerDraw.gather_objects(),
            &discardTemp,
            &opponentDraw.gather_objects()
        },
        true,
        false)
{

    // Now we set up our selectables
    // We give it a base selection of just the hand
    menu.set_selectables(selection_range[BJ_Target::HAND]);
    menu.set_selector(&slct);
    // set our floater to nullptr
    floater = nullptr;
}

// Destructor. Gathers all sprites and deletes them
Board::~Board() {
    for (auto& o : gather_objects())
        delete o;
}

// Gathers all sprites in the BlackjackPlayer for storage in the ObjectManager.
lni::vector<geng::Sprite*> Board::gather_objects() {
    lni::vector<geng::Sprite*> allObjects;
    allObjects.reserve(100);
    // First we grab all of the easy sprites
    allObjects.push_back(&slct);
    allObjects.push_back(&deck);
    // Then we need to make sure to grab all the sub sprites.
    // First the deck
    allObjects.insert(allObjects.end(), deck.gather_objects().begin(), deck.gather_objects().end());
    // Then the player draw
    allObjects.insert(allObjects.end(), playerDraw.gather_objects().begin(), playerDraw.gather_objects().end());
    // Then the opponent draw
    allObjects.insert(allObjects.end(), opponentDraw.gather_objects().begin(), opponentDraw.gather_objects().end());
    // Finally we return the vector.
    return allObjects;
}

bool Board::add_card_to_hand(Card *c) {
    return hand.add_card(c);
}

void Board::update() {
    pather.update(deck);
}
