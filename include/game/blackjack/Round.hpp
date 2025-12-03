#pragma once
#include "Board.hpp"
#include "engine/input/InputTarget.hpp"

namespace blackjack {

    class Round final : public gengine::InputTarget {
        // Player scores
        int playerScore = 0;
        int opponentScore = 0;
        // We'll need a pointer to the board.
        Board* board;
        // Checks if we're ready to start the round.
        bool acceptingInput = false;
    public:
        // Keeps time so we can separate out events
        BJ_Result result = BJ_Result::NONE;
        bool done = false;
        // Constructor
        explicit Round(Board* board);

        // Beginning round
        void begin();
        // Opponent's turn
        void opponent_turn();
        // Ending the round
        void round_end();

        void refresh_deck();

        // Checks if we've gotten a blackjack
        void check_blackjack();
        // Gets a keypress or release
        bool get_press(gengine::GENG_Input keybind) override;
        void get_release(gengine::GENG_Input keybind) override;
    };
}