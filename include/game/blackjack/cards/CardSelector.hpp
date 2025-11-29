#pragma once

#include "game/blackjack/BjConstants.hpp"
#include "game/blackjack/cards/Card.hpp"
#include "menu/Selector.hpp"
#include "scene/textures/asset-info/RegistryConstants.hpp"

// CardSelector Object!
struct CardSelector : Selector<Card> {
    using Selector::target;
    short targetNum = 0;
    // Constructor
    CardSelector();

    // Tells the selector to move to it's target card. Recess if you want the pointer closer
    void move(bool recess) override;
    void move(Vertex pos, float speed) override;

    // Lets us switch targets
    void switchTarget(Card* newTarget, bool moveTarget) override;

    // Sets the color of the pointer
    void set_color(uint8_t color);
    // Lets us deactivate the pointer for a period of time
    void deactivate() override;
    // Then we need to say we shake if we're out of bounds
    void shake_target() override;
    // Returns the target
    Card* get_target();
};

/*                              */

/* Defaults and macros for the Card Selector */
// Default pos
#define SELECTOR_DEFAULT_POS Vertex(scene::width/2, 13*scene::height/16, SELECTOR_Z_BASE)
// Different definitions for our selector colors!
#define SELECTOR_COLOR_NUMS 6
#define SELECTOR_COLOR_YELLOW 0
#define SELECTOR_COLOR_GREY 1
#define SELECTOR_COLOR_PURPLE 2
#define SELECTOR_COLOR_GREEN 3
#define SELECTOR_COLOR_RED 4
#define SELECTOR_COLOR_PINK 5

// Default transform properties for our object
const static Transform card_selector_transform = {
    SELECTOR_DEFAULT_POS,
    8,
    8,
    1,
    SELECTOR_Z_BASE
};