#pragma once
#include "Constants.hpp"
#include <vector>
#include <unordered_map>
#include "generics/Transform.hpp"

namespace blackjack {
    inline int roundScore = 21;
    inline short maxHandSize = 12;
    inline uint8_t maxStackSize = 3;
}

/* Definitions and Macros for BlackJack Player */
// Current Target
#define BJ_TARGET_HAND 0
#define BJ_TARGET_PLAYER 1
#define BJ_TARGET_DISCARD 2
#define BJ_TARGET_OPPONENT 3
#define BJ_TARGET_DISCARD_MENU 4
#define BJ_TARGET_DRAWS 5
#define BJ_TARGET_HAND_AND_DRAWS 6
#define BJ_TARGET_ALL 7
// Current Selection Mode
#define BJ_SELECT_PLAY 0    // Play a card to draw
#define BJ_SELECT_PULL 1    // Pull a card to hand
#define BJ_SELECT_PUSH 2    // Push a card to opponent draw
#define BJ_SELECT_DESTROY 3 // Destroy a card
#define BJ_SELECT_OPEN_DISCARD 4    // Open discard pile
#define BJ_SELECT_USE_CARD 5    // Use a card's ability
#define BJ_SELECT_GRAB 6   //Grabs a card
// Selection offset
#define BJ_SELECT_OFFSET Vertex(0, 5, 0)
#define BJ_SELECTOR_OFFSET Vertex(0, 5, 0)
// Selection pathing
#define BJ_SELECT_PATH PATH_BALLOON

// Default positions
// CardDraws
#define BJ_DEFAULT_PLAYER_POS Vertex(3.3*scene::width/8, 3.4*scene::height/8.0, 0)
#define BJ_DEFAULT_OPPONENT_POS Vertex(3.3*scene::width/8, scene::height/8.0, 0)
// Floater card
#define BJ_DEFAULT_FLOATER_POS Vertex(scene::width/4.0f, scene::height/6.0f, 1)
// Deck & discard
#define BJ_DEFAULT_DECK_POS Vertex(7*scene::width/8, 2.3*scene::height/8, 0)
#define BJ_DEFAULT_DISCARD_POS Vertex(scene::width/8.0f, 1.2*scene::height/3.0f, 2)


// Special Card Types
#define SPECIAL_CARD_SUIT 'x'
#define BJ_TAKER_CARD 54
#define BJ_INCREMENT_CARD 55
#define BJ_DECREMENT_CARD 56
#define BJ_PUSH_CARD 57
#define BJ_PULL_CARD 58

// Increment Card
#define CARD_INCREMENT_VALUE 3
// For denying action
#define BJ_SHAKE_DENY SHAKE_SIDE, 3.0, 300, 0.5
#define BJ_SHAKE_FLOATER SHAKE_FLOATY, 3.0, -1, 2.0, false
/* Z-INDEXING */
// Z-indexing for blackjack
#define SELECTOR_Z_BASE 13.0f
#define HAND_Z_BASE 10.0f
#define BOARD_Z_BASE 9.0f
#define DEAL_Z_BASE 8.0f
#define DECK_Z_BASE 7.0f

// Maps target selections to menuRegion selection regions
static std::unordered_map<short,std::vector<short>> selection_range = {
    {BJ_TARGET_HAND, {BJ_TARGET_HAND}},
    {BJ_TARGET_PLAYER, {BJ_TARGET_PLAYER}},
    {BJ_TARGET_OPPONENT, {BJ_TARGET_OPPONENT}},
    {BJ_TARGET_DRAWS, {BJ_TARGET_PLAYER, BJ_TARGET_OPPONENT}},
    {BJ_TARGET_HAND_AND_DRAWS, {BJ_TARGET_HAND, BJ_TARGET_PLAYER, BJ_TARGET_OPPONENT}},
    {BJ_TARGET_ALL, {BJ_TARGET_HAND, BJ_TARGET_PLAYER, BJ_TARGET_DISCARD, BJ_TARGET_DRAWS}}
};

// Default Object Setups
static const Transform default_deck = {
    BJ_DEFAULT_DECK_POS,
    53,
    37,
    1,
    DECK_Z_BASE
};