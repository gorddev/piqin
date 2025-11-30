#include "game/blackjack/BlackjackPlayer.hpp"

using namespace blackjack;

bool BlackjackPlayer::getPress(gengine::GENG_Input keybind) {
    // First we just see if we chose to select something or not.
    if (keybind == gengine::GENG_Input::MODIFY) {
        hand.flayed = true;
        hand.update_cards();
        slct.switchTarget(slct.get_target(), true);
        update_selector_color();
        return true;
    }
    if (keybind == gengine::GENG_Input::SELECT_ALT) {
        select();
        return true;
    }
    // If we back out, we need to do a bunch of stuff
    if (keybind == gengine::GENG_Input::BACK) {
        back();
        return true;
    }
    if (keybind == gengine::GENG_Input::UP) {
        if (mnu.move_up())
            update_selector_color();
        return true;
    }
    if (keybind == gengine::GENG_Input::DOWN) {
        if (mnu.move_down())
            update_selector_color();
        return true;
    }
    if (keybind == gengine::GENG_Input::LEFT&& hand.flayed) {
        if (mnu.move_left())
            update_selector_color();
        if (mnu.get_row() == 0)
            hand.lastTarget = mnu.get_column();
        return true;
    }
    if (keybind == gengine::GENG_Input::RIGHT&& hand.flayed) {
        if (mnu.move_right())
            update_selector_color();
        if (mnu.get_row() == 0)
            hand.lastTarget = mnu.get_column();
        return true;
    }
    if (keybind == gengine::GENG_Input::SELECT) {
        playerDraw.add_card(deck.pop_card());
    }
    return false;
}

void BlackjackPlayer::getRelease(gengine::GENG_Input keybind) {
    if (keybind == gengine::GENG_Input::MODIFY && range == BJ_Target::HAND) {
        hand.flayed = false;
        hand.update_cards();
        slct.move(true);
        update_selector_color();
    }
}
