#pragma once
#include <engine/gan.hpp>

#include "Affix.hpp"
#include "Suit.hpp"
#include "engine/mods/animation/sprites/Sprite.hpp"

namespace card {

    class Card : public gan::Sprite{
        /// All the affixes attached to the card
        std::vector<Affix> affixes;
        /// Value of the card
        uint32_t value;
        /// Suit of the card
        Suit suit;
        /// Whether the card is flipped or not
        bool flipped;

        /// Returns the animatino number for the card for internal indexing purposes
        uint16_t get_anim_num();
        /// Normalizes the cards so all values are within range
        void normalize();
    public:
        /// Constructor for the card
        Card(gan::FrameTable &frame_table, uint32_t value, Suit suit);

        // <><><> Flipping <><><>
        /// Flips a card up so that it is visible
        void flip_up();
        /// Flips a card down so that is in not visible
        void flip_down();
        /// Flips a card opposite of it's current orientation
        void flip();

        // <><><> Validation <><><>
        /// Returns true if the card is flipped
        [[nodiscard]] bool is_flipped() const;
        /// Returns true if this is a special card
        [[nodiscard]] bool is_special() const;
        /// Returns the suit of the card
        [[nodiscard]] Suit get_suit() const;

        // <><><> Changing values <><><>
        /// Sets the value of the card
        bool set_value(int val);
        /// Increments the value of a card by the provided value
        bool incr_value_by(int val);

        // <><><> Scoring <><><>
        /// Gets the score of the card
        [[nodiscard]] long get_score();
    };
}
