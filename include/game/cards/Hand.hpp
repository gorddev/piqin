#pragma once
#include "ObjectManager.hpp"
#include "input/InputTarget.hpp"
#include "game/cards/Card.hpp"

#define MAX_HAND_SIZE 8
#define HAND_Z_BASE 10.0f
#define HAND_DEFAULT_SPEED 1.0f
// The hand someone can play cards from
class Hand : public InputTarget {
private:
    // The cards they have access to
    std::vector<Card*> cards;
    // The visual pointer to those cards
    Object* card_selector;
    // The current user's target card that the selector points to
    uint8_t targetCard = 0;
    // The previous target (for graphical purposes)
    uint8_t oldTarget = 0;

    Path get_card_path(int index, bool selected,
        int& center, int& height,
        float speed = HAND_DEFAULT_SPEED,
        Vertex mod = Vertex(0,0,0));

    // Removes cards with the given properties
    Card* pop_card(uint8_t value, char suite);
    // Removes a card with the given cardnum
    Card* pop_card(uint8_t cardNum);
public:
    Hand();
    // Adds a card to the hand if there's space
    bool add_card(Card* c);
    // Update the cards in the Hand with new paths
    void update_cards(bool force = false);
    // Allows the game master to pop a card
    Card* pop_card();
    // Whether the hand is flayed for selection or not
    bool flayed = false;
    // Tells the game manager when to pop a card onto the player hand
    bool popcard = false;

    // Gets a pointer to the selector
    [[nodiscard]] Object* get_selector() const;

    int get_num_cards() const;

    // INPUT TARGET OVERRIDE
    bool getPress(short keybind) override;

};