#include "../../../../include/engine/particles/particle-types/ParticleRhombus.hpp"
#include "game/blackjack/Board.hpp"

using namespace blackjack;

// Lets us know if a card is usable or not
bool Board::usable(Card *c) {
    if (c->get_suit() != BJ_Suit::SPECIAL) {
        return !playerDraw.will_bust(c);
    }
    if (c->get_value() == BJ_CARD_INCREMENT || c->get_value() == BJ_CARD_DECREMENT)
        return (!playerDraw.empty() || !opponentDraw.empty());
    if (c->get_value() == BJ_CARD_PUSH)
        return (!playerDraw.empty());
    if (c->get_value() == BJ_CARD_PULL)
        return (!opponentDraw.empty());
    if (c->get_value() == BJ_CARD_TAKER)
        return (!opponentDraw.empty() || !playerDraw.empty() || !discard.empty());
    return false;
}

void Board::play_card() {
    // If we aren't selecting return nothing
    if (slct.get_target() == nullptr || hand.empty())
        return;
    // If the card is a special card, we try and do special shit
    if (slct.get_target()->get_suit() == BJ_Suit::SPECIAL)
        apply_special_card(hand[menu.s.column]);
    // Otherwise we try and play the card as usual
    else if (slct.get_target()->get_suit() != BJ_Suit::SPECIAL) {
        // First we make sure we dont bust
        if (playerDraw.will_bust(slct.get_target())) {
            // If we do we shake
            slct.get_target()->set_shake(BJ_SHAKE_DENY);
            return;
        }
        // Otherwise We add the card as normal to the hand
        pather.move(hand.pop_card(menu.s.column), playerDraw);
        pather.update_hand(hand);
    }
    // Always update our selector and color for some reason
    menu.update_selector();
    update_selector();
}


// Applying our special cards so we can use them.
void Board::apply_special_card(Card *c) {
    // If we're not usable, we immediately return.
    if (!usable(c)) {
        c->set_shake(BJ_SHAKE_DENY);
        return;
    }
    // we first establish it as a floater card
    floater = hand.pop_card(menu.col());
    // send the floater to where it needs to go.
    pather.to_floater(floater);
    // update cards in the hand
    pather.update_hand(hand);

    // We need to first make sure we can use our card.
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
        menu.move_up();
    }
    else {
        set_target_range(BJ_Target::HAND);
        action = BJ_Action::PLAY;
    }
}