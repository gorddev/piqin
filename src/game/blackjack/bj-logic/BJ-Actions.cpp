//#include "engine/particles/ParticleCircle.hpp"
#include "../../../../include/engine/particles/particle-types/ParticleSparkle.hpp"
#include "game/blackjack/Board.hpp"

using namespace blackjack;

#define ADDEV(t, f) gan::GENG_Events.add_event(t, [=]{f;})

void Board::select() {
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

void Board::back() {
    // First check if our floater is a nullptr
    if (floater != nullptr) {
        // If not, we add it back to hand
        add_card_to_hand(floater);
        // Remove it's shaking & particle animation
        floater->remove_shake();
        bob.detach_particle(floater);
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
        update_selector();
        // Reset our action to play from the hand
        action = BJ_Action::PLAY;
        set_target_range(BJ_Target::HAND);
    }
}

void Board::use_card() {
    if (floater != nullptr && floater->use(slct.get_target()))
        deactivate_floater();
}

void Board::pull_card() {
    Card* c = nullptr;
    if (menu.row() == BJ_Target::DISCARD_MENU)
        c = deck.pop_card();
    else if (menu.row() == BJ_Target::OPPONENT)
        c = opponentDraw.pop_card(menu.col());
    if (c != nullptr) {
        //auto* pc = new gan::ParticleCircle(c, 20.0, 0.125, 200, 20, {180, 220, 180, 255});
        //bob.attach_new_particle(c, pc);
        pather.move(c, playerDraw);
    }
    deactivate_floater();
}

void Board::push_card() {
    Card* c = nullptr;
    if (menu.row() == BJ_Target::HAND)
        c = hand.pop_card(menu.col());
    else if (menu.row() == BJ_Target::PLAYER)
        c = playerDraw.pop_card(menu.col());
    if (c != nullptr) {
        //auto* pc = new gan::ParticleCircle(c, 20.0, 0.125, 200, 20, {180, 220, 180, 255});
        //bob.attach_new_particle(c, pc);
        pather.move(c, opponentDraw);
    }
    deactivate_floater();
}

void Board::grab_card() {
    Card* c = nullptr;
    if (menu.row() == BJ_Target::DISCARD_MENU)
        c = discard.pop_card();
    else if (menu.row() == BJ_Target::OPPONENT)
        c = opponentDraw.pop_card(menu.col());
    else if (menu.row() == BJ_Target::PLAYER)
        c = playerDraw.pop_card(menu.col());
    if (c != nullptr) {
        auto* ps = new gan::ParticleSparkle(floater, 8.0f, 1.0, 350, 30);
        bob.attach_new_particle(c, ps);
        pather.move(c, hand);
        pather.update_hand(hand);
    }

    deactivate_floater();
}

void Board::set_target_range(BJ_Target targetRange) {
    range = targetRange;
    menu.set_selectables(selection_range[range]);
    update_selector();
}

void Board::set_target_action(BJ_Action new_action) {
    this->action = new_action;
}

void Board::deactivate_floater() {
    bob.detach_particle(floater);
    floater->set_shake(gan::GENG_Shake::RANDOM,3.0,550,1,true);
    ADDEV(600, auto* ps = new gan::ParticleSparkle(floater, 8.0f, 1.0, 350, 30);
        bob.attach_new_particle(floater, ps);
        pather.to_discard(floater, &discard, gan::GENG_Path::SINE);
        floater = nullptr);
    set_target_action(BJ_Action::PLAY);
    set_target_range(BJ_Target::HAND);
}