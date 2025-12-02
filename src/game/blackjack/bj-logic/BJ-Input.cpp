#include "game/blackjack/Player.hpp"

using namespace blackjack;

bool Player::getPress(gengine::GENG_Input keybind) {
    // First we just see if we chose to select something or not.
    if (keybind == gengine::GENG_Input::MODIFY) {
        hand.flayed = true;
        pather.update_hand(hand);
        slct.switchTarget(slct.get_target(), true);
        update_selector_color();
        return true;
    }
    if (keybind == gengine::GENG_Input::SELECT_ALT && hand.flayed) {
        select();
        return true;
    }
    // If we back out, we need to do a bunch of stuff
    if (keybind == gengine::GENG_Input::BACK) {
        back();
        return true;
    }
    if (keybind == gengine::GENG_Input::UP) {
        if (menu.move_up())
            update_selector_color();
        return true;
    }
    if (keybind == gengine::GENG_Input::DOWN) {
        if (menu.move_down())
            update_selector_color();
        return true;
    }
    if (keybind == gengine::GENG_Input::LEFT&& hand.flayed) {
        if (menu.move_left())
            update_selector_color();
        if (menu.get_row() == 0)
            hand.lastTarget = menu.col();
        return true;
    }
    if (keybind == gengine::GENG_Input::RIGHT&& hand.flayed) {
        if (menu.move_right())
            update_selector_color();
        if (menu.get_row() == 0)
            hand.lastTarget = menu.col();
        return true;
    }
    if (keybind == gengine::GENG_Input::SELECT) {
        pather.move(deck.pop_card(), playerDraw);
    }
    return false;
}

void Player::getRelease(gengine::GENG_Input keybind) {
    if (keybind == gengine::GENG_Input::MODIFY && range == BJ_Target::HAND) {
        hand.flayed = false;
        pather.update_hand(hand);
        slct.move(true);
        update_selector_color();
    }
}

void Player::update_selector_color() {
    if (!hand.flayed || hand.empty())
        slct.set_color(Selector_Color::GREY);
    else if (!usable(slct.get_target()))
        slct.set_color(Selector_Color::RED);
    else if (slct.get_target()->get_suit() == BJ_Suit::SPECIAL)
        slct.set_color(Selector_Color::GREEN);
    else
        slct.set_color(Selector_Color::YELLOW);
}