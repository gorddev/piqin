#pragma once
#include "cards/CardSelector.hpp"
#include "game/blackjack/Hand.hpp"
#include "cards/CardDraw.hpp"
#include "cards/Deck.hpp"
#include "BJEnums.hpp"
#include "EngineSource.hpp"
#include "engine/input/menu/MenuRegion.hpp"

namespace blackjack {
    // We must be able to input to our InputTarget
    class BlackjackPlayer : public gengine::InputTarget {
        /* Member Objects */
        // We need to be able to select cards
        CardSelector slct;
        // We need access to the deck
        Deck deck;
        // We need acces to the player hand
        Hand hand;
        // Then, we need to keep drack of the draws for both the player and opponent
        CardDraw playerDraw;
        CardDraw opponentDraw;
        // Finally, we have a floater card if we're using a special card
        Card* floater;

        // This lets us keep track of the menu
        gengine::MenuRegion<Card> mnu;
        // Creates a menu region wrapper (dont worry about it)
        std::vector<Card*> discardTemp;

        /* Member Variables */
        BJ_Target target = BJ_Target::HAND;
        BJ_Target range = BJ_Target::HAND;
        BJ_Action action = BJ_Action::PLAY;
        bool selectEnabled = true;

    public:
        // Public constructor
        BlackjackPlayer();
        ~BlackjackPlayer() override;
        // Gathers all the objects in the BlackjackPlayer class.
        [[nodiscard]] std::vector<gengine::Object*> gather_objects();

        /* CARD_SELECTOR */
        /* Functions that concern the selector */
        void update_selector_color();

        /* Using Special Cards */
        void apply_special_card(Card* c);

        /* HAND */
        /* Functions that concern management of the hand */
        bool add_card_to_hand(Card* c);
        void play_card_from_hand();


        /* INPUT HANDLING */
        bool getPress(gengine::GENG_Input keybind) override;
        void getRelease(gengine::GENG_Input keybind) override;

    private:
        // Tells us if a card is usable or not
        bool usable(Card* c);
        // Lets us use a card for various purposes
        //void move_card(uint8_t location); // Moves a card to a loaction
        // Functions that cards use
        void use_card();    // Uses a card's ability
        void pull_card();   // Pulls a card from a location to the draw
        void grab_card();   // Grabs a card and puts it to the hand
        void push_card();   // Pushes a card to the opponent's craw

        // Updates our selection parameters
        void set_target_range(BJ_Target targetRange);
        void set_target_action(BJ_Action action);

        // When we select something, we call this function
        void select();
        void back();
    };
}