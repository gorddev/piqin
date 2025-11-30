#include "game/blackjack/BlackjackPlayer.hpp"

using namespace blackjack;

BlackjackPlayer::BlackjackPlayer() :
        playerDraw(BJ_DEFAULT_PLAYER_POS),
        opponentDraw(BJ_DEFAULT_OPPONENT_POS),
        slct(), hand(), discardTemp(1),
        mnu(
        {
            &hand.cards,
            &playerDraw.card_list(),
            &discardTemp,
            &opponentDraw.card_list()
        },
        true,
        false)
{

    // Not sure what this line is for but it seems important.
    discardTemp[0] = &deck.discard;
    // Now we set up our selectables
    // We give it a base selection of just the hand
    mnu.set_selectables(selection_range[BJ_Target::HAND]);
    mnu.set_selector(&slct);

    // set our floater to nullptr
    floater = nullptr;
}

// Destructor. Gathers all objects and deletes them
BlackjackPlayer::~BlackjackPlayer() {
    for (auto& o : gather_objects())
        delete o;
}

// Gathers all objects in the BlackjackPlayer for storage in the ObjectManager.
std::vector<gengine::Object*> BlackjackPlayer::gather_objects() {
    std::vector<gengine::Object*> allObjects;
    allObjects.reserve(100);
    // First we grab all of the easy objects
    allObjects.push_back(&slct);
    allObjects.push_back(&deck);
    // Then we need to make sure to grab all the sub objects.
    // First the deck
    allObjects.insert(allObjects.end(), deck.gather_objects().begin(), deck.gather_objects().end());
    // Then the player draw
    allObjects.insert(allObjects.end(), playerDraw.card_list().begin(), playerDraw.card_list().end());
    // Then the opponent draw
    allObjects.insert(allObjects.end(), opponentDraw.card_list().begin(), opponentDraw.card_list().end());
    // Finally we return the vector.
    return allObjects;
}

void BlackjackPlayer::play_card_from_hand() {
    // If we aren't selecting return nothing
    if (slct.get_target() == nullptr || hand.empty())
        return;
    // If the card is a special card, we try and do special shit
    if (slct.get_target()->get_suit() == BJ_Suit::SPECIAL)
        apply_special_card(hand[mnu.s.column]);
    // Otherwise we try and play the card as usual
    else if (slct.get_target()->get_suit() != BJ_Suit::SPECIAL) {
        // First we make sure we dont bust
        if (playerDraw.will_bust(slct.get_target())) {
            // If we do we shake
            slct.get_target()->set_shake(BJ_SHAKE_DENY);
            return;
        }
        // Otherwise We add the card as normal to the hand
        playerDraw.add_card(hand.pop_card(mnu.s.column));
        hand.update_cards();
    }
    // Always update our selector and color for some reason
    mnu.update_selector();
    update_selector_color();
}


bool BlackjackPlayer::add_card_to_hand(Card *c) {
    return hand.add_card(c);
}
