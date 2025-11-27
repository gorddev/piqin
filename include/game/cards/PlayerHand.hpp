#pragma once
#include "CardSelector.hpp"
#include "ObjectManager.hpp"
#include "input/InputTarget.hpp"
#include "game/cards/Card.hpp"
#include "game/board/Deal.hpp"

#define MAX_HAND_SIZE 12
#define HAND_Z_BASE 10.0f
#define HAND_DEFAULT_SPEED 1.0f
// The hand someone can play cards from
class PlayerHand : public InputTarget {
private:
    // The cards they have access to
    std::vector<Card*> cards;
    // The visual pointer to those cards
    CardSelector* selector;
    // The hand they are currently dealt
    Deal* deal;
    // The current user's target card that the selector points to
    uint8_t targetCard = 0;
    // The old target card
    uint8_t oldTarget = 0;

    Path get_card_path(int index,
        int& center, int& height,
        float speed = HAND_DEFAULT_SPEED,
        Vertex mod = Vertex(0,0,0));

    // Removes a card with the given cardnum
    Card* pop_hand(uint8_t cardNum);
public:
    PlayerHand();
    ~PlayerHand();
    /* FUNCTIONS TO DO WITH YOUR HAND */
    // Adds a card to the hand if there's space
    bool add_card_to_hand(Card* c);
    // Update the cards in the Hand with new paths
    void update_cards(bool force = false);
    // Allows the game master to pop a card
    Card* pop_hand();
    // Pops a card from the hand to the deal
    void play_card();

    // Whether the hand is flayed for selection or not
    bool flayed = false;
    // Request a hit on the deal
    bool hit = false;
    // Request to stay
    bool stay = false;

    // Gets a pointer to the selector
    [[nodiscard]] Object* get_hand_selector() const;
    // Returns then number of cards
    [[nodiscard]] int get_num_cards() const;

    /* FUNCTIONS TO DO WITH DEALT HAND */
    // Returns the deal of the hand
    Deal* get_deal();
    // Grabs a card from the deal to the hand
    void take_card();

    // INPUT TARGET OVERRIDE
    bool getPress(short keybind) override;

};