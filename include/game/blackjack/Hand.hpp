#pragma once
#include "cards/CardSelector.hpp"
#include "game/blackjack/cards/Card.hpp"
#include "game/blackjack/cards/CardDraw.hpp"

#define HAND_Z_BASE 10.0f
#define HAND_DEFAULT_SPEED 1.0f
namespace blackjack {
    // The hand someone can play cards from
    struct Hand {
        /* -----------------*/
        /* Member variables */
        /* -----------------*/
        // The cards they have access to
        std::vector<Card*> cards;
        // Whether the hand is flayed for selection or not
        bool flayed = false;
        // Maintains the last target.
        short lastTarget;

        /* -----------------*/
        /* Const/dstr  */
        /* -----------------*/
        Hand();

        /* -----------------*/
        /* Member functions */
        /* -----------------*/
        /* Adding/removing cards */
        // Checks if the hand is full (can check for stackable cards)
        bool full(uint8_t cardType = 0) const;
        bool empty() const;
        int size() const;
        // Adds a card to the hand if there's space
        bool add_card(Card* c);
        // Removes a card with the given cardnum
        Card* pop_card(int cardNum);


        // Gets path to the hand.
        gengine::Path get_card_path(int index,
            int& center, int& height,
            float speed = HAND_DEFAULT_SPEED,
            gengine::Vertex mod = gengine::Vertex(0,0,0)) const;
        /* FUNCTIONS TO DO WITH YOUR HAND */
        // Update the cards in the Hand with new paths
        void update_cards();

        bool has_normal_cards();

        /* Operators */
        Card* operator[](int index) const;
    };
}