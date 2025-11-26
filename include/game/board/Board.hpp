#pragma once
#include "Deal.hpp"
#include "game/cards/Card.hpp"
#include "input/InputTarget.hpp"
#include <vector>

#define BOARD_Z_BASE 9.0f
#define MAX_DECK_SIZE 100
class Board : public InputTarget {
private:
    // The deck with all the cards
    std::vector<Card*> deck;
    // Position of the deck
    Vertex deckPos;
    // Player character's deal
    Deal player = Deal(Vertex(3*scene::width/8, 7*scene::height/16, 0));
    // Opponent's dea8
    Deal opponent = Deal(Vertex(3*scene::width/8, scene::height/8, 0));
    // Number it takes to get blackjack
    int targetScore = 21;

    // Returns the path of the card back to the deck.
    Path get_card_path(Card*& c) const;
    // Allows us to remove a card from the vector
    bool remove_card(Card* c);
    // Removes a card from the deck by popping it
    [[nodiscard]] Card* pop_card(int i);

public:
    // Pointer back to player's hand
    bool switch_to_hand = false;
    // Lets the gameMaster to know when to switch to the hand again.

    // Default constructor, creates 52 cards
    Board();
    // Gets the deck
    [[nodiscard]] std::vector<Card*>& get_deck();
    // Adds a card to the deck
    void add_card(Card* c);
    // Adds a card to the player deal
    bool hit_player();
    // Adds a specific card to the player deal
    bool hit_player(Card* c);
    // Adds a card to the opponent deal
    bool hit_opponent();
    // Adds a specific card to the opponent deal
    bool hit_opponent(Card* c);

    // Pops a card from the deck

    // Gets the cards back from both deals
    void retrieve_cards();

    // See scores and who wins
    void comp_scores();

    // Input Target overrides
    // Gets the press so we can decide what to do
    bool getPress(short keybind) override;

};