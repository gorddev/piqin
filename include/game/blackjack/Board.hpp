#pragma once
#include "bj-objects/CardSelector.hpp"
#include "bj-objects/Pather.hpp"
#include "BJEnums.hpp"
#include "EngineSource.hpp"
#include "engine/input/menu/MenuRegion.hpp"

namespace blackjack {
    // We must be able to input to our InputTarget
    class Board : public gengine::InputTarget {
    public:
        /* Member Objects */
        // We need to be able to select cards
        CardSelector slct;
        // We need access to the deck
        Deck deck;
        // Discard pile
        Discard discard;
        // We need acces to the player hand
        Hand hand;
        // Then, we need to keep drack of the draws for both the player and opponent
        Draw playerDraw;
        Draw opponentDraw;
        // Finally, we have a floater card if we're using a special card
        Card* floater;
    private:
        // This lets us keep track of the menu
        gengine::MenuRegion<Card> menu;
        // Creates a menu region wrapper (dont worry about it)
        std::vector<Card*> discardTemp;

        /* Member Variables */
        BJ_Target target = BJ_Target::HAND;
        BJ_Target range = BJ_Target::HAND;
        BJ_Action action = BJ_Action::PLAY;
        bool selectEnabled = true;

    public:

        // Public constructor
        Board();
        ~Board() override;
        // Gathers all the objects in the BlackjackPlayer class.
        [[nodiscard]] std::vector<gengine::Object*> gather_objects();

        /* CARD_SELECTOR */
        /* Functions that concern the selector */
        void update_selector();

        /* HAND */
        /* Functions that concern management of the hand */
        bool add_card_to_hand(Card* c);

        /* INPUT HANDLING */
        bool get_press(gengine::GENG_Input keybind) override;
        void get_release(gengine::GENG_Input keybind) override;

        // Updates
        void update();

    private:
        // Tells us if a card is usable or not
        bool usable(Card* c);
        /* Using Special Cards */
        void apply_special_card(Card* c);
        // Using various cards on other cards
        void play_card();   // Plays a card to the draw
        void use_card();    // Uses a card's ability
        void pull_card();   // Pulls a card from a location to the draw
        void grab_card();   // Grabs a card and puts it to the hand
        void push_card();   // Pushes a card to the opponent's craw

        // Updates our selection parameters
        void set_target_range(BJ_Target targetRange);
        void set_target_action(BJ_Action new_action);

        // De-initializes the floater and puts it back into the hand
        void uninitialize_floater();

        // When we select something, we call this function
        void select();
        void back();
    };
}