#pragma once

#include "engine/input/menu/Selector.hpp"
#include "game/blackjack/BJEnums.hpp"
#include "game/blackjack/cards/Card.hpp"
#include "game/blackjack/BJConstants.hpp"

namespace blackjack {

    enum class Selector_Color : uint8_t {
        YELLOW = 0,
        GREY = 1,
        PURPLE = 2,
        GREEN = 3,
        RED = 4,
        PINK = 5
    };
    // CardSelector Object!
    struct CardSelector : gengine::Selector<Card> {
        using Selector::target;
        short targetNum = 0;
        // Constructor
        CardSelector();

        // Tells the selector to move to it's target card. Recess if you want the pointer closer
        void move(bool recess) override;
        void move(gengine::Vertex pos, float speed) override;

        // Lets us switch targets
        void switchTarget(Card* newTarget, bool moveTarget) override;

        // Sets the color of the pointer
        void set_color(Selector_Color color);
        // Lets us deactivate the pointer for a period of time
        void deactivate() override;
        // Then we need to say we shake if we're out of bounds
        void shake_target() override;
        // Returns the target
        Card* get_target();

        void remove_target();
    };

    /*                              */
    // Default transform properties for our object
    const static gengine::Transform card_selector_transform = {
        BJ_DEFAULT_SELECTOR_POS, // NOLINT(*-narrowing-conversions)
        8,
        8,
        1
    };
}