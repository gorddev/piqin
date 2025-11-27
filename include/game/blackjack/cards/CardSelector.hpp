#pragma once

#include "game/blackjack/cards/Card.hpp"
#include "scene/textures/asset-info/RegistryConstants.hpp"

#define SELECTOR_Z_BASE 20.0f
#define SELECTOR_DEFAULT_POS Vertex(scene::width/2.0, 13*scene::height/16, SELECTOR_Z_BASE)
// Different definitions for our selector colors!
#define SELECTOR_COLOR_NUMS 5
#define SELECTOR_COLOR_YELLOW 0
#define SELECTOR_COLOR_GREY 1
#define SELECTOR_COLOR_PURPLE 2
#define SELECTOR_COLOR_GREEN 3
#define SELECTOR_COLOR_RED 4
class CardSelector : public Object {

public:
    // Constructor
    CardSelector();

    // Tells the selector to move to a certain position.
    void move(Vertex pos, double speed = 1.0);
    void move(Path p);
    void move(Card* c, Vertex offset = Vertex(0,0,0));
    // Sets the color of the pointer
    void set_color(uint8_t color);
    // Lets us deactivate the pointer for a period of time
    void deactivate(bool returnCard = true);
};


const static Transform card_selector_transform = {
    Vertex(scene::width/2.0, scene::height/2.0, 0),
    8,
    8,
    1,
    SELECTOR_Z_BASE
};