#include "game/blackjack/BlackjackPlayer.hpp"

// Lets us know if a card is usable or not
bool BlackjackPlayer::usable(Card *c) const {
    if (c->get_suit() != SPECIAL_CARD_SUIT) {
        return !playerDraw->will_bust(c);
    }
    if (c->get_value() == BJ_INCREMENT_CARD || c->get_value() == BJ_DECREMENT_CARD)
        return (hand->has_normal_cards() || playerDraw->size() != 0);
    if (c->get_value() == BJ_PUSH_CARD)
        return (playerDraw->size() != 0);
    if (c->get_value() == BJ_PULL_CARD)
        return (opponentDraw->size() != 0);
    if (c->get_value() == BJ_TAKER_CARD)
        return (!opponentDraw->empty() || !playerDraw->empty() || !deck->discardPile.empty());
    return false;
}

void BlackjackPlayer::use_card() {
    if (floater != nullptr) {
        if (floater->use(slct->get_target())) {
            floater->set_shake(SHAKE_FLOATY, 5.0, 450, 2.0, true);
            deck->discard_card(floater);
            floater = nullptr;
            set_target_access(BJ_TARGET_HAND);
            set_target_mode(BJ_SELECT_PLAY);
        }
    }
}

void BlackjackPlayer::pull_card() {
    if (mnu->get_row() == BJ_TARGET_DISCARD_MENU)
        playerDraw->add_card(deck->pop_discard_card(mnu->get_column()));
    else if (mnu->get_row() == BJ_TARGET_OPPONENT)
        playerDraw->add_card(opponentDraw->pop_card(mnu->get_column()));

    floater->set_shake(SHAKE_FLOATY, 5.0, 450, 2.0, true);
    deck->discard_card(floater);
    floater = nullptr;
    set_target_access(BJ_TARGET_HAND);
    set_target_mode(BJ_SELECT_PLAY);
}

void BlackjackPlayer::grab_card() {
    if (mnu->get_row() == BJ_TARGET_DISCARD_MENU)
        hand->add_card(deck->pop_discard_card(mnu->get_column()));
    else if (mnu->get_row() == BJ_TARGET_OPPONENT)
        hand->add_card(opponentDraw->pop_card(mnu->get_column()));
    else if (mnu->get_row() == BJ_TARGET_PLAYER)
        hand->add_card(playerDraw->pop_card(mnu->get_column()));

    floater->set_shake(SHAKE_FLOATY, 5.0, 450, 2.0, true);
    deck->discard_card(floater);
    floater = nullptr;
    set_target_access(BJ_TARGET_HAND);
    set_target_mode(BJ_SELECT_PLAY);
}

void BlackjackPlayer::push_card() {
    if (mnu->get_row() == BJ_TARGET_HAND)
        opponentDraw->add_card(hand->pop_card(mnu->get_column()));
    else if (mnu->get_row() == BJ_TARGET_PLAYER)
        opponentDraw->add_card(playerDraw->pop_card(mnu->get_column()));
    floater->set_shake(SHAKE_FLOATY, 5.0, 450, 2.0, true);
    deck->discard_card(floater);
    floater = nullptr;
    set_target_access(BJ_TARGET_HAND);
    set_target_mode(BJ_SELECT_PLAY);
}

// Applying our special cards so we can use them.
void BlackjackPlayer::apply_special_card(Card *c) {
    // If we're not usable, we immediately return.
    if (!usable(c)) {
        c->set_shake(BJ_SHAKE_DENY);
        return;
    }
    // we first establish it as a floater card
    floater = hand->pop_card(mnu->get_column());
    // send the floater to where it needs to go.
    floater->set_path(BJ_DEFAULT_FLOATER_POS, PATH_BALLOON, 1.0, true);
    floater->set_shake(BJ_SHAKE_FLOATER);
    // update cards in the hand
    hand->update_cards();

    // We need to first make sure we can use our card.
    bool pass = false;
    if (floater->get_value() == BJ_TAKER_CARD) {
        set_target_access(BJ_TARGET_DRAWS);
        selectMode = BJ_SELECT_GRAB;
    }
    else if (floater->get_value() == BJ_PUSH_CARD) {
        set_target_access(BJ_TARGET_PLAYER);
        selectMode = BJ_SELECT_PUSH;
    }
    else if (floater->get_value() == BJ_PULL_CARD) {
        set_target_access(BJ_TARGET_OPPONENT);
        selectMode = BJ_SELECT_PULL;
    }
    else if (floater->get_value() == BJ_DECREMENT_CARD || floater->get_value() == BJ_INCREMENT_CARD) {
        set_target_access(BJ_TARGET_HAND_AND_DRAWS);
        selectMode = BJ_SELECT_USE_CARD;
        mnu->move_up();
    }
    else {
        set_target_access(BJ_TARGET_HAND);
        selectMode = BJ_SELECT_PLAY;
    }
}