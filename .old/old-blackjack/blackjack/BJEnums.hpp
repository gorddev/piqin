#pragma once

#include <vector>
#include <unordered_map>
#include "engine/types/Transform2D.hpp"
#include "BJConstants.hpp"

namespace blackjack {
    inline int roundScore = 21;
    inline short maxHandSize = 12;
    inline uint8_t maxStackSize = 3;
}

/* Definitions and Macros for BlackJack Player */
// Current Target
namespace blackjack {
    // Lets us target different parts of the board
    enum class BJ_Target : int {
        HAND = 0,
        PLAYER = 1,
        DISCARD = 2,
        OPPONENT = 3,
        DISCARD_MENU = 4,
        DRAWS = 5,
        HAND_AND_DRAWS = 6,
        ALL = 7
    };

    bool inline operator==(char c, BJ_Target target) {
        return c == static_cast<char>(target);
    }
    bool inline operator==(BJ_Target target, char c) {
        return static_cast<char>(target) == c;
    }
    // Lets us do different actions when we select a card
    enum class BJ_Action : uint8_t {
        PLAY = 0,           // Play a card from hand
        PULL = 1,           // Pull card from opponent to your draw
        PUSH = 2,           // Push card from draw to opponent
        DESTROY = 3,        // Destroy Card
        OPEN_DISCARD = 4,   // Open the discard pile
        USE_CARD = 5,       // Use a card's ability
        GRAB = 6            // Grab a card to your hard
    };
    enum class BJ_Suit: char {
        SPADE = 's',
        CLUB = 'c',
        DIAMOND = 'd',
        HEART = 'h',
        SPECIAL = 'x'
    };
    enum BJ_CardValue : uint8_t {
        BJ_CARD_NONE = 0,
        BJ_CARD_TAKER = 54,
        BJ_CARD_INCREMENT = 55,
        BJ_CARD_DECREMENT = 56,
        BJ_CARD_PUSH = 57,
        BJ_CARD_PULL = 58
    };
    enum class BJ_Result : uint8_t {
        NONE = 0,
        LOSE = 1,
        TIE = 2,
        WIN = 3
    };

    // Maps target selections to menuRegion selection regions
    static std::unordered_map<BJ_Target,std::vector<short>> selection_range = {
        {BJ_Target::HAND, {static_cast<short>(BJ_Target::HAND)}},
        {BJ_Target::PLAYER, {static_cast<short>(BJ_Target::PLAYER)}},
        {BJ_Target::OPPONENT, {static_cast<short>(BJ_Target::OPPONENT)}},
        {BJ_Target::DRAWS, {static_cast<short>(BJ_Target::PLAYER), static_cast<short>(BJ_Target::OPPONENT)}},
        {BJ_Target::HAND_AND_DRAWS, {static_cast<short>(BJ_Target::HAND), static_cast<short>(BJ_Target::PLAYER), static_cast<short>(BJ_Target::OPPONENT)}},
        {BJ_Target::ALL, {static_cast<short>(BJ_Target::HAND), static_cast<short>(BJ_Target::PLAYER), static_cast<short>(BJ_Target::DISCARD), static_cast<short>(BJ_Target::DRAWS)}}
    };

    // Default Object Setups
    static const gan::Transform2D default_deck = {
        {0,0},
        53,
        37,
        1,
    };
}

