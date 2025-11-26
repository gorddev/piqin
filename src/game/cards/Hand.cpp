#include "game/cards/Hand.hpp"

#include <iostream>
#include <ostream>

#include "ObjectManager.hpp"
#include "input/InputConstants.hpp"

Hand::Hand() {
    targetCard = 0;

    // Setting up the little selector
    card_selector = new Object({30,30,30});
    card_selector->set_sheet_id(ASSET_CARD_SELECTOR_ID);
    card_selector->set_width(8);
    card_selector->set_height(8);
    card_selector->set_scale(1);
    card_selector->set_z_index(HAND_Z_BASE+0.6f);
}

bool Hand::add_card(Card* c) {
    if (cards.size() <= MAX_HAND_SIZE) {
        c->set_z_index(HAND_Z_BASE+cards.size()/(MAX_HAND_SIZE*2.0f)); // NOLINT(*-narrowing-conversions)
        cards.push_back(c);
        update_cards(true);
    }
    else
        return false;
    return true;
}

Card* Hand::pop_card(uint8_t cardNum) {
    if (cardNum < cards.size()) {
        Card* ret = cards[cardNum];
        cards.erase(cards.begin()+cardNum);
        if (cards.size() != 0 && targetCard != 0)
            targetCard--;
        update_cards(true);
        return ret;
    }
    return nullptr;
}

Card * Hand::pop_card() {
    if (!popcard) return nullptr;
    std::cerr<<"pop_card()"<<std::endl;
    return pop_card(targetCard);
}

Path Hand::get_card_path(int index, bool selected, int& center, int& height, float speed, Vertex mod) {
    int flay = (is_held(INPUT_MODIFY)) ? (20) : 0;
    Path p = {
        {
            // Space each card more x-wise if hand is flayed
            center + (14+flay) * (index - ((cards.size()-1)/2.0f)) + mod[0], // NOLINT(*-narrowing-conversions)
            // Set default height. If not flayed, stagger each card vertically from the last picked.
            (height - (flay*1.25)) - (flay/4.0f) + ((!flay) * abs(targetCard-index)*2) - (selected*5) + mod[1], // NOLINT(*-narrowing-conversions)
            Z_HAND_BASE + index/(MAX_HAND_SIZE*2.0f) + mod[2] // NOLINT(*-narrowing-conversions)
        },
        cards[index]->pos(),
        PATH_BALLOON,
        speed
    };
    return p;
}

void Hand::update_cards(bool force) {
    int center = scene::width/2;
    int height = 3*scene::height/4;
    if (cards.empty())
        card_selector->hidden = true;
    else if (flayed != is_held(INPUT_MODIFY) || force) {
        for (int i = 0; i < cards.size(); i++)
            cards[i]->set_path(get_card_path(i, i == targetCard, center, height));
        flayed = is_held(INPUT_MODIFY);
        // Dark or light
        card_selector->set_state(!flayed);
        if (!flayed)
            card_selector->set_path(get_card_path(targetCard,true, center, height, HAND_DEFAULT_SPEED, Vertex(0,15,0)));
        else
            card_selector->set_path(get_card_path(targetCard,true, center, height,HAND_DEFAULT_SPEED , Vertex(0,10,0)));
    }
    else if (targetCard != oldTarget) {
        // Push the cards up/ down if they're selected or not
        cards[targetCard]->set_path(get_card_path(targetCard, true, center, height, 0.125));
        cards[oldTarget]->set_path(get_card_path(oldTarget, false, center, height, 0.125));
        // Update our pointer accordingly
        card_selector->set_path(get_card_path(targetCard,true, center, height,0.2, Vertex(0,10,0)));
    }
    oldTarget = targetCard;
}

bool Hand::getPress(short keybind) {
    if (keybind == INPUT_RIGHT) {
        targetCard = (targetCard + 1) % cards.size();
        return true;
    }
    if (keybind == INPUT_LEFT) {
        if ((targetCard - 1) < 0)
            targetCard = cards.size() - 1;
        else
            targetCard--;
        return true;
    }
    if (keybind == INPUT_SELECT_ALT) {

        popcard = true;
        return true;
    }

    return false;
}

Object* Hand::get_selector() const {
    return card_selector;
}

int Hand::get_num_cards() const {
    return cards.size();
}