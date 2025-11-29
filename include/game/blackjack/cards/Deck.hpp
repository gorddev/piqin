#pragma once

#include "generics/Object.hpp"
#include "game/blackjack/cards/Card.hpp"
#include <vector>

#define MAX_DECK_SIZE 100.0f
#define STANDARD_DECK_SIZE 52
class Deck: public Object {
private:
    // Keeps track of cards traveling back to the deck
    std::vector<Card*> traveling;
    // Gives us the number of cards back on the deck.
    int cardsOnDeck;
public:
    // Keeps track of cards in the deck
    std::vector<Card*> drawPile;
    // Allows us to discard cards to the side
    std::vector<Card*> discardPile;
    Card discard;

private:
    // Gets the path to the deck
    [[nodiscard]] Path get_path_deck(Vertex pos) const;
    // What we do upon card arrival to deck
    void card_arrival(Card* c);
    // What we call upon card removal from deck
    void card_removal(Card* c, int i);
    // Gets the current pixel-height of the deck.
    [[nodiscard]] short deck_height() const;
public:
    Deck();
    ~Deck();

    /* Deck Management */
    // Initialize card means no travel time and goes straight in deck
    void initialize_card(Card* c);
    // Add card means that the card travels to the deck
    void add_card(Card* c);
    void add_cards(std::vector<Card*> cards);
    // Pops a card off the top of the deck.
    Card* pop_card();
    // Updates traveling cards
    void update_travel_cards();

    /* Discard Management */
    // Adds a card to the discard pile
    void discard_card(Card* c, int cardNum = 0);
    void discard_cards(std::vector<Card*> cards);
    // Refreshes the paths of each card in the discard pile
    void refresh_discard_pile();
    // Pops the entire discard pile back to the deck
    void pop_discard_pile();

    Card *pop_discard_card(int num);

    // Sends cards back to the draw pile.

    // Gets the pointers for loading into objectManager
    std::vector<Card*>& gather_objects();



};