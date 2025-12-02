#include "game/blackjack/Player.hpp"

using namespace blackjack;

void Player::play() {

}



void Player::select() {
    // If we're currently playing a card
    if (action == BJ_Action::PLAY) {
        if (target == BJ_Target::HAND)
            play_card();
    }
    else if (action == BJ_Action::USE_CARD)
        use_card();
    else if (action == BJ_Action::PULL)
        pull_card();
    else if (action == BJ_Action::PUSH)
        push_card();
    else if (action == BJ_Action::GRAB)
        grab_card();
}

void Player::back() {
    // First check if our floater is a nullptr
    if (floater != nullptr) {
        // If not, we add it back to hand
        add_card_to_hand(floater);
        // Remove it's shaking animation
        floater->remove_shake();
        // Remove it's particle effect
        bob.remove_attached_particle(floater);
        // Turn it into nullpointer
        floater = nullptr;
        // Unflay the hand
        hand.flayed = false;
        // Update selector selection
        menu.update_selector();
        // Update our hand's card paths
        pather.update_hand(hand);
        //  Move our selector back
        slct.move(true);
        // Update the selector's color
        update_selector_color();
        // Reset our action to play from the hand
        action = BJ_Action::PLAY;
        // Force our target range back into the hand.
        set_target_range(BJ_Target::HAND);
    }
}

void Player::use_card() {
    if (floater != nullptr && floater->use(slct.get_target()))
        uninitialize_floater();
}

void Player::pull_card() {
    if (menu.get_row() == BJ_Target::DISCARD_MENU)
        pather.move(deck.pop_card(), playerDraw);
    else if (menu.get_row() == BJ_Target::OPPONENT)
        pather.move(opponentDraw.pop_card(), playerDraw);
    uninitialize_floater();
}

void Player::push_card() {
    if (menu.get_row() == BJ_Target::HAND)
        pather.move(hand.pop_card(menu.col()), opponentDraw);
    else if (menu.get_row() == BJ_Target::PLAYER)
        pather.move(playerDraw.pop_card(menu.col()), opponentDraw);
    uninitialize_floater();
}

void Player::grab_card() {
    if (menu.get_row() == BJ_Target::DISCARD_MENU)
        pather.move(discard.pop_card(), hand, 0);
    else if (menu.get_row() == BJ_Target::OPPONENT)
        pather.move(opponentDraw.pop_card(menu.col()), opponentDraw);
    else if (menu.get_row() == BJ_Target::PLAYER)
        pather.move(playerDraw.pop_card(menu.col()), playerDraw);
    uninitialize_floater();
}

void Player::set_target_range(BJ_Target targetRange) {
    range = targetRange;
    menu.set_selectables(selection_range[range]);
    update_selector_color();
}

void Player::set_target_action(BJ_Action new_action) {
    this->action = new_action;
}

void Player::uninitialize_floater() {
    pather.to_discard(floater, discard);
    set_target_action(BJ_Action::PLAY);
    set_target_range(BJ_Target::HAND);
}