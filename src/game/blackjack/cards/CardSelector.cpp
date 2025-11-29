#include "game/blackjack/cards/CardSelector.hpp"

#include <iostream>
#include <ostream>

#include "game/blackjack/BjConstants.hpp"

CardSelector::CardSelector() {
    target = nullptr;
    t = card_selector_transform;
    fs.sheet_id = ASSET_CARD_SELECTOR_ID;
}


// Changes cards. Default pathing = path true
void CardSelector::switchTarget(Card *newTarget, bool moveTarget) {

    if (target != nullptr && target->get_path()!= nullptr && target->tag == TAG_SELECTOR) {
        target->set_path(target->get_path()->get_target() + BJ_SELECT_OFFSET, BJ_SELECT_PATH, 0.7);
        target->tag = TAG_SELECTOR;
    }
    else if (target != nullptr && target->get_path() == nullptr) {
        target->set_path(target->pos() + BJ_SELECT_OFFSET, BJ_SELECT_PATH, 0.7);
    }
    target = newTarget;

    if (moveTarget) {
        if (target->get_path() != nullptr) {
            target->set_path(target->get_path()->get_target() - BJ_SELECT_OFFSET, BJ_SELECT_PATH, 0.7);
        }
        else {
            target->set_path(target->pos() - BJ_SELECT_OFFSET, BJ_SELECT_PATH, 0.7);
        }
        target->tag = TAG_SELECTOR;
    }
    move(moveTarget);
}

// DOES NOT UPDATE A CARD'S PATH, JUST SELECTOR
void CardSelector::move(bool recess) {
    Vertex os = (recess) ? BJ_SELECTOR_OFFSET : BJ_SELECTOR_OFFSET * -1;
    if (target == nullptr)
        deactivate();
    else if (target->get_path() != nullptr) {
        move(target->get_path()->get_target() + os, 1.0);
    }
    else
        move(target->pos(), 1.0);
}

// DOES NOT UPDATE A CARD'S PATH, JUST SELECTOR
void CardSelector::move(Vertex pos, float speed) {
    set_path(Path(pos, t.pos, BJ_SELECT_PATH, speed));
}

void CardSelector::set_color(uint8_t color) {
    if (color > SELECTOR_COLOR_NUMS) color = SELECTOR_COLOR_GREY;
    fs.state = color;
}

void CardSelector::deactivate() {
    move(SELECTOR_DEFAULT_POS, 1.0);
    set_color(SELECTOR_COLOR_GREY);
}

// nothing for now
void CardSelector::shake_target() {
    target->set_shake(BJ_SHAKE_DENY);
}

Card * CardSelector::get_target() {
    return target;
}
