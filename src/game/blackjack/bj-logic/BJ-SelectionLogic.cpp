#include "game/blackjack/BlackjackPlayer.hpp"
#include "menu/MenuRegion.hpp"
#include "input/InputConstants.hpp"

void BlackjackPlayer::update_selector_color() const {
    if (!hand->flayed || hand->empty())
        slct->set_color(SELECTOR_COLOR_GREY);
    else if (!usable(slct->get_target()))
        slct->set_color(SELECTOR_COLOR_RED);
    else if (slct->get_target()->get_suit() == SPECIAL_CARD_SUIT)
        slct->set_color(SELECTOR_COLOR_GREEN);
    else
        slct->set_color(SELECTOR_COLOR_YELLOW);
}

void BlackjackPlayer::set_target_access(uint8_t range) {
    targetAccess = range;
    mnu->set_selectables(selection_range[range]);
    update_selector_color();
}

void BlackjackPlayer::set_target_mode(uint8_t mode) {
    selectMode = mode;
}

void BlackjackPlayer::select() {
    // If we're currently playing a card
    if (selectMode == BJ_SELECT_PLAY) {
        if (target == BJ_TARGET_HAND)
            play_card_from_hand();
    }
    else if (selectMode == BJ_SELECT_USE_CARD)
        use_card();
    else if (selectMode == BJ_SELECT_PULL)
        pull_card();
    else if (selectMode == BJ_SELECT_PUSH)
        push_card();
    else if (selectMode == BJ_SELECT_GRAB)
        grab_card();

}

void BlackjackPlayer::back() {

    if (floater != nullptr) {
        add_card_to_hand(floater);
        floater->remove_shake();
        floater = nullptr;
        hand->flayed = false;
        mnu->update_selector();
        hand->update_cards();
        slct->move(true);
        update_selector_color();
        selectMode = BJ_SELECT_PLAY;
        set_target_access(BJ_SELECT_PLAY);
    }
}