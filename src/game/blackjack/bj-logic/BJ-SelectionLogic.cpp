#include "game/blackjack/BlackjackPlayer.hpp"

using namespace blackjack;

void BlackjackPlayer::update_selector_color() {
    if (!hand.flayed || hand.empty())
        slct.set_color(Selector_Color::GREY);
    else if (!usable(slct.get_target()))
        slct.set_color(Selector_Color::RED);
    else if (slct.get_target()->get_suit() == BJ_Suit::SPECIAL)
        slct.set_color(Selector_Color::GREEN);
    else
        slct.set_color(Selector_Color::YELLOW);
}

void BlackjackPlayer::set_target_range(BJ_Target targetRange) {
    range = targetRange;
    mnu.set_selectables(selection_range[range]);
    update_selector_color();
}

void BlackjackPlayer::set_target_action(BJ_Action action) {
    this->action = action;
}

void BlackjackPlayer::select() {
    // If we're currently playing a card
    if (action == BJ_Action::PLAY) {
        if (target == BJ_Target::HAND)
            play_card_from_hand();
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

void BlackjackPlayer::back() {
    // First check if our floater is a nullptr
    if (floater != nullptr) {
        // If not, we add it back to hand
        add_card_to_hand(floater);
        // Remove it's shaking animation
        floater->remove_shake();
        // Turn it into nullpointer
        floater = nullptr;
        // Unflay the hand
        hand.flayed = false;
        // Update selector selection
        mnu.update_selector();
        // Update our hand's card paths
        hand.update_cards();
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