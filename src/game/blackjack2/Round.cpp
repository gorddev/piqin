#include "game/blackjack/Round.hpp"
#include "EngineSource.hpp"
#include "engine/gengine-globals/Timer.hpp"
#include "engine/pathing/path-types/BalloonPath.hpp"

using namespace blackjack;

// Easy macro for adding events
#define ADDEV(t, f) gengine::GENG_Events.add_event(t, [=]{f;})

Round::Round(Board* board) : board(board) {}

void Round::begin() {

    if (board->deck.size() < 4)
        refresh_deck();
    // First add a card to the opponent's hand
    ADDEV(600,pather.move(board->deck.pop_card(), board->opponentDraw,  0, true));
    // Then wait and add a second card
    ADDEV(300, pather.move(board->deck.pop_card(), board->opponentDraw));
    // Then wait again and add to add to the player hand
    ADDEV(600, pather.move(board->deck.pop_card(), board->playerDraw));
    // Wait once again
    ADDEV(300, pather.move(board->deck.pop_card(), board->playerDraw));
    // Then we go to round start
    ADDEV(400, acceptingInput = true; check_blackjack());
}

void Round::check_blackjack() {
    bool op = board->opponentDraw.get_score() == roundScore;
    bool p = board->playerDraw.get_score() == roundScore;
    if (op) {
        if (p)
            result = BJ_Result::TIE;
        else
            result = BJ_Result::LOSE;
        round_end();
    }
    if (p) {
        result = BJ_Result::WIN;
        round_end();
    }
    pather.update_hand(board->hand);
}

void Round::opponent_turn() {
    acceptingInput = false;
    std::cerr << board->opponentDraw.gather_objects().size() << std::endl;
    if (board->opponentDraw.get_score() < roundScore - 4) {
        pather.move(board->deck.pop_card(), board->opponentDraw);
        ADDEV(500, opponent_turn());
    }
    else
        ADDEV(600, round_end());
}

void Round::get_release(gengine::GENG_Input keybind) {
    if (acceptingInput)
        board->get_release(keybind);
}

void Round::round_end() {
    // Give all the cards a good shake
    ADDEV(200, board->opponentDraw.gather_objects()[0]->flip_up());

    int k = 0;
    lni::vector<Card*>& pdr = board->playerDraw.gather_objects();
    lni::vector<Card*>& opr = board->opponentDraw.gather_objects();
    for (auto& i: board->playerDraw.gather_objects())
        ADDEV(std::max(abs(120 - 5*k++), 10), bob.apply_effect(*i, BJ_SHAKE_END_ROUND));
    for (auto& i: board->opponentDraw.gather_objects())
        ADDEV(std::max(abs(120 - 5*k++), 10), bob.apply_effect(*i, BJ_SHAKE_END_ROUND));


    ADDEV(750,
        lni::vector<Card*> cs = board->playerDraw.pop_cards(); lni::vector<Card*> os = board->opponentDraw.pop_cards();
        pather.to_discard(cs, &board->discard); pather.to_discard(os, &board->discard);
        done = true
    );
}


void Round::refresh_deck() {
    pather.move(board->discard.pop_all(), &board->deck);
    ADDEV(200, );
}


bool Round::get_press(gengine::GENG_Input keybind) {
    if (acceptingInput) {
        pather.update_hand(board->hand);
        if (keybind == gengine::GENG_Input::BACK && board->floater == nullptr) {
            board->hand.flayed = false;
            pather.update_hand(board->hand);
            board->update_selector();
            acceptingInput = false;
            opponent_turn();
            return true;
        }
        return board->get_press(keybind);
    }
    return false;
}