#pragma once
#include "cards/Deal.hpp"
#include "game/blackjack/cards/Card.hpp"
#include "game/blackjack/cards/Deck.hpp"
#include <vector>

#define BOARD_Z_BASE 9.0f
class Board {
private:
    // The deck with all the cards
    Deck deck;
    // Position of the deck
    Vertex deckPos;
    // Player character's deal
    Deal* player = nullptr;
    // Opponent's dea8
    Deal* opponent = nullptr;
    // Number it takes to get blackjack
    int targetScore = 21;

    // Removes a card from the deck by popping it
    [[nodiscard]] Card* pop_deck(int i);

public:
    // Pointer back to player's hand
    bool switch_to_hand = false;
    // Lets the gameMaster to know when to switch to the hand again.

    // Default constructor, creates 52 cards
    explicit Board(Deal* player);
    ~Board();
    // Gets the deck vector
    [[nodiscard]] std::vector<Card*>& get_deck();
    // Gets the deck object
    Deck* get_deck_obj();
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

    // DECK MANAGEMENT
    // Allows us to pop a card from the deck
    Card* pop_deck_card();
    // Allows us to add elements to the deck
    void add_deck_card(Card* c);
    // ALlows us to add multiple elements to the deck
    void add_deck_cards(std::vector<Card*>& cards);

    // Gets the cards back from both deals
    void retrieve_cards();

    // See scores and who wins
    void comp_scores();

};