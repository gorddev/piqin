#pragma once

#include "blackjack/BlackjackPlayer.hpp"
#include "game/blackjack/Hand.hpp"

class GameMaster {
private:
    BlackjackPlayer* player;
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