#pragma once
#include "cards/CardSelector.hpp"
#include "game/blackjack/Hand.hpp"
#include "input/InputTarget.hpp"
#include "cards/CardDraw.hpp"
#include "cards/Deck.hpp"
#include "menu/MenuRegion.hpp"
#include "BjConstants.hpp"

// We must be able to input to our InputTarget
class BlackjackPlayer : public InputTarget {
    /* Member Objects */
    // We need to be able to select cards
    CardSelector* slct;
    // We need access to the deck
    Deck* deck;
    // We need acces to the player hand
    Hand* hand;
    // Then, we need to keep drack of the draws for both the player and opponent
    CardDraw* playerDraw;
    CardDraw* opponentDraw;
    // Finally, we have a floater card if we're using a special card
    Card* floater{};

    // This lets us keep track of the menu
    MenuRegion<Card>* mnu;
    // Creates a menu region wrapper (dont worry about it)
    std::vector<Card*>* discardTemp;

    /* Member Variables */
    uint8_t target = BJ_TARGET_HAND;
    uint8_t targetAccess = BJ_TARGET_HAND;
    uint8_t selectMode = BJ_SELECT_PLAY;
    bool selectEnabled = true;

public:
    // Public constructor
    BlackjackPlayer();
    ~BlackjackPlayer() override;
    // Gathers all the objects in the BlackjackPlayer class.
    std::vector<Object*> gather_objects() const;

    /* CARD_SELECTOR */
    /* Functions that concern the selector */
    void update_selector_color() const;

    /* Using Special Cards */
    void apply_special_card(Card* c);

    /* HAND */
    /* Functions that concern management of the hand */
    bool add_card_to_hand(Card* c);
    void play_card_from_hand();


    /* INPUT HANDLING */
    bool getPress(short keybind) override;
    void getRelease(short keybind) override;

private:
    // Tells us if a card is usable or not
    bool usable(Card* c) const;
    // Lets us use a card for various purposes
    void move_card(uint8_t location); // Moves a card to a loaction
    // Functions that cards use
    void use_card();    // Uses a card's ability
    void pull_card();   // Pulls a card from a location to the draw
    void grab_card();   // Grabs a card and puts it to the hand
    void push_card();   // Pushes a card to the opponent's craw

    // Updates our selection parameters
    void set_target_access(uint8_t range);
    void set_target_mode(uint8_t mode);

    // When we select something, we call this function
    void select();
    void back();

};


