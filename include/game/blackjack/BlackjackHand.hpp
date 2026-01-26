#pragma once
#include "../cards/single-cards/Card.hpp"

namespace bj {

    class BlackjackHand {
    private:
        /// Contains all the cards within the hand
        std::vector<card::Card*> cards;
        /// Contains the hover banner for the card

        /// Contains the target position of all cards in the deck
        gan::pos2 position;
    public:
        /// Default constructor
        BlackjackHand(gan::pos2 pos);

        // <><><> Adding/removing cards <><><>
        /// Removes a card from the hand via pointer
        card::Card* pop_card(card::Card* card);
        /// Removes a card from the hand via index
        card::Card* pop_card(uint_fast16_t index);
        /// Adds a card to the hand
        void add_card(card::Card* card);

        // <><><> Scoring <><><>
        /// Returns the current score of the hand
        long get_score();
    };
}
