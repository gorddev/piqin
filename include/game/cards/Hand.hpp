#pragma once
#include "ObjectManager.hpp"
#include "input/InputTarget.hpp"
#include "game/cards/Card.hpp"

#define MAX_HAND_SIZE 8
class Hand : public InputTarget {
private:
    std::vector<Card*> cards;
    Object* card_selector;
    uint8_t numCards = 0;
    uint8_t targetCard = 0;
    uint8_t oldTarget = 0;
    uint8_t flayed = 0;

    Path get_card_path(int index, bool selected, int& center, int& height, Vertex mod = Vertex(0,0,0));
public:
    Hand();
    // Adds a card to the hand if there's space
    bool add_card(Card* c);
    bool remove_card(uint8_t value, char suite);
    void update_cards(bool force = false);
    bool getPress(short keybind);
    [[nodiscard]] Object* get_pointer() const;

};