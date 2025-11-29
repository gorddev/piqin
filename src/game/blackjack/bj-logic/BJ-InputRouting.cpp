#include "game/blackjack/BlackjackPlayer.hpp"
#include "input/InputConstants.hpp"

bool BlackjackPlayer::getPress(short keybind) {
    // First we just see if we chose to select something or not.
    if (keybind == INPUT_MODIFY) {
        hand->flayed = true;
        hand->update_cards();
        slct->switchTarget(slct->get_target(), true);
        update_selector_color();
        return true;
    }
    if (keybind == INPUT_SELECT_ALT) {
        select();
        return true;
    }
    // If we back out, we need to do a bunch of stuff
    if (keybind == INPUT_BACK) {
        back();
        return true;
    }
    if (keybind == INPUT_UP) {
        if (mnu->move_up())
            update_selector_color();
        return true;
    }
    if (keybind == INPUT_DOWN) {
        if (mnu->move_down())
            update_selector_color();
        return true;
    }
    if (keybind == INPUT_LEFT&& hand->flayed) {
        if (mnu->move_left())
            update_selector_color();
        if (mnu->get_row() == 0)
            hand->lastTarget = mnu->get_column();
        return true;
    }
    if (keybind == INPUT_RIGHT&& hand->flayed) {
        if (mnu->move_right())
            update_selector_color();
        if (mnu->get_row() == 0)
            hand->lastTarget = mnu->get_column();
        return true;
    }
    if (keybind == INPUT_SELECT) {
        playerDraw->add_card(deck->pop_card());
    }
    return false;
}

void BlackjackPlayer::getRelease(short keybind) {
    if (keybind == INPUT_MODIFY && targetAccess == BJ_TARGET_HAND) {
        hand->flayed = false;
        hand->update_cards();
        slct->move(true);
        update_selector_color();
    }
}
