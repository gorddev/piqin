#pragma once

#include "game/cards/PlayerHand.hpp"
#include "game/board/Board.hpp"

class GameMaster {
private:
    PlayerHand* hand = nullptr;
    Board* board = nullptr;
public:
    GameMaster() = default;
    ~GameMaster();
    void initialize();

    // Updating the objects
    void update();
    // Dealing with the Hand
    void add_card_to_hand(Card c);
    // Set input target
    void set_hand_as_target();

    void blackjack();
};