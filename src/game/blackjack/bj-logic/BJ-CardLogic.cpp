#include "game/blackjack/BlackjackPlayer.hpp"

using namespace blackjack;

// Lets us know if a card is usable or not
bool BlackjackPlayer::usable(Card *c) {
    if (c->get_suit() != BJ_Suit::SPECIAL) {
        return !playerDraw.will_bust(c);
    }
    if (c->get_value() == BJ_CARD_INCREMENT || c->get_value() == BJ_CARD_DECREMENT)
        return (hand.has_normal_cards() || !playerDraw.empty());
    if (c->get_value() == BJ_CARD_PUSH)
        return (!playerDraw.empty());
    if (c->get_value() == BJ_CARD_PULL)
        return (!opponentDraw.empty());
    if (c->get_value() == BJ_CARD_TAKER)
        return (!opponentDraw.empty() || !playerDraw.empty() || !deck.discardPile.empty());
    return false;
}

void BlackjackPlayer::use_card() {
    if (floater != nullptr) {
        if (floater->use(slct.get_target())) {
            floater->set_shake(gengine::GENG_Shake::FLOATY, 5.0, 450, 2.0, true);
            deck.discard_card(floater);
            floater = nullptr;
            set_target_range(BJ_Target::HAND);
            set_target_action(BJ_Action::PLAY);
        }
    }
}

void BlackjackPlayer::pull_card() {
    if (mnu.get_row() == BJ_Target::DISCARD_MENU)
        playerDraw.add_card(deck.pop_discard_card(mnu.get_column()));
    else if (mnu.get_row() == BJ_Target::OPPONENT)
        playerDraw.add_card(opponentDraw.pop_card(mnu.get_column()));

    floater->set_shake(gengine::GENG_Shake::FLOATY, 5.0, 450, 2.0, true);
    deck.discard_card(floater);
    floater = nullptr;
    set_target_range(BJ_Target::HAND);
    set_target_action(BJ_Action::PLAY);
}

void BlackjackPlayer::grab_card() {
    if (mnu.get_row() == BJ_Target::DISCARD_MENU)
        hand.add_card(deck.pop_discard_card(mnu.get_column()));
    else if (mnu.get_row() == BJ_Target::OPPONENT)
        hand.add_card(opponentDraw.pop_card(mnu.get_column()));
    else if (mnu.get_row() == BJ_Target::PLAYER)
        hand.add_card(playerDraw.pop_card(mnu.get_column()));

    floater->set_shake(gengine::GENG_Shake::FLOATY, 5.0, 450, 2.0, true);
    deck.discard_card(floater);
    floater = nullptr;
    set_target_range(BJ_Target::HAND);
    set_target_action(BJ_Action::PLAY);
}

void BlackjackPlayer::push_card() {
    if (mnu.get_row() == BJ_Target::HAND)
        opponentDraw.add_card(hand.pop_card(mnu.get_column()));
    else if (mnu.get_row() == BJ_Target::PLAYER)
        opponentDraw.add_card(playerDraw.pop_card(mnu.get_column()));
    floater->set_shake(gengine::GENG_Shake::FLOATY, 5.0, 450, 2.0, true);
    deck.discard_card(floater);
    floater = nullptr;
    set_target_range(BJ_Target::HAND);
    set_target_action(BJ_Action::PLAY);
}

// Applying our special cards so we can use them.
void BlackjackPlayer::apply_special_card(Card *c) {
    // If we're not usable, we immediately return.
    if (!usable(c)) {
        c->set_shake(BJ_SHAKE_DENY);
        return;
    }
    // we first establish it as a floater card
    floater = hand.pop_card(mnu.get_column());
    // send the floater to where it needs to go.
    floater->set_path(BJ_DEFAULT_FLOATER_POS, gengine::GENG_Path::BALLOON, 1.0, true);
    floater->set_shake(BJ_SHAKE_FLOATER);
    // update cards in the hand
    hand.update_cards();

    // We need to first make sure we can use our card.
    bool pass = false;
    if (floater->get_value() == BJ_CARD_TAKER) {
        set_target_range(BJ_Target::DRAWS);
        action = BJ_Action::GRAB;
    }
    else if (floater->get_value() == BJ_CARD_PUSH) {
        set_target_range(BJ_Target::PLAYER);
        action = BJ_Action::PUSH;
    }
    else if (floater->get_value() == BJ_CARD_PULL) {
        set_target_range(BJ_Target::OPPONENT);
        action = BJ_Action::PULL;
    }
    else if (floater->get_value() == BJ_CARD_DECREMENT || floater->get_value() == BJ_CARD_INCREMENT) {
        set_target_range(BJ_Target::HAND_AND_DRAWS);
        action = BJ_Action::USE_CARD;
        mnu.move_up();
    }
    else {
        set_target_range(BJ_Target::HAND);
        action = BJ_Action::PLAY;
    }
}