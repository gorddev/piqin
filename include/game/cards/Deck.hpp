#pragma once

#include "generics/Object.hpp"
#include "game/cards/Card.hpp"
#include <vector>

#define MAX_DECK_SIZE 100.0f
#define STANDARD_DECK_SIZE 52
class Deck: public Object {
private:
    std::vector<Card*> deck;
    std::vector<Card*> traveling;
    int cardsOnDeck;

    Path get_path(Vertex pos) const;
    void card_arrival();
    void card_removal();
    short deck_height() const;
public:
    Deck();

    // Initialize card means no travel time and goes straight in deck
    void initialize_card(Card* c);
    // Add card means that the card travels to the deck
    void add_card(Card* c);
    // Same with add_cards
    void add_cards(std::vector<Card*> cards);
    // Pops a card off the top of the deck.
    Card* pop_card();
    // Updates traveling cards
    void update_travel_cards();

    // Gets the pointers for loading into objectManager
    std::vector<Card*>& get_deck();
};