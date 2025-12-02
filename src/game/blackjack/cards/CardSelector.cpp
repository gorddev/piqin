#include "game/blackjack/cards/CardSelector.hpp"

#include <iostream>
#include <ostream>

#include "game/blackjack/BJEnums.hpp"

using namespace blackjack;

CardSelector::CardSelector() {
    target = nullptr;
    t = card_selector_transform;
    fs.sheet_id = ASSET_CARD_SELECTOR_ID;
}


// Changes cards. Default pathing = path true
void CardSelector::switchTarget(Card *newTarget, bool moveTarget) {

    if (target != nullptr && target->get_path()!= nullptr && target->tag == gengine::GENG_Tag::SELECTOR) {
        gengine::Vertex trueTarget = target->get_path()->get_target();
        trueTarget.z = target->get_path()->get_target().z;
        target->set_path(trueTarget + BJ_SELECT_OFFSET, BJ_SELECT_PATH, 0.7);
        target->tag = gengine::GENG_Tag::SELECTOR;
    }
    else if (target != nullptr && target->get_path() == nullptr) {
        gengine::Vertex trueTarget = target->pos();
        trueTarget.z = target->pos().z;
        target->set_path(trueTarget + BJ_SELECT_OFFSET, BJ_SELECT_PATH, 0.7);
    }
    else if (newTarget == nullptr) {
        target = nullptr;
        deactivate();
        return;
    }
    target = newTarget;

    if (moveTarget) {
        if (target->get_path() != nullptr) {
            target->set_path(target->get_path()->get_target() - BJ_SELECT_OFFSET, BJ_SELECT_PATH, 0.7);
        }
        else {
            target->set_path(target->pos() - BJ_SELECT_OFFSET, BJ_SELECT_PATH, 0.7);
        }
        target->tag = gengine::GENG_Tag::SELECTOR;
    }
    move(moveTarget);
}

// DOES NOT UPDATE A CARD'S PATH, JUST SELECTOR
void CardSelector::move(bool recess) {
    gengine::Vertex os = (recess) ? BJ_SELECTOR_OFFSET : BJ_SELECTOR_OFFSET * -1;
    if (target == nullptr)
        deactivate();
    else if (target->get_path() != nullptr) {
        gengine::Vertex trueTarget = target->get_path()->get_target();
        trueTarget.z = SELECTOR_Z_BASE;
        move(trueTarget + os, 1.0);
    }
    else {
        gengine::Vertex trueTarget = target->get_path()->get_target();
        trueTarget.z = SELECTOR_Z_BASE;
        move(target->pos(), 1.0);
    }
}

// DOES NOT UPDATE A CARD'S PATH, JUST SELECTOR
void CardSelector::move(gengine::Vertex pos, float speed) {
    gengine::Vertex trueTarget = pos;
    trueTarget.z = SELECTOR_Z_BASE;
    set_path(gengine::Path(trueTarget, t.pos, BJ_SELECT_PATH, speed));
}

void CardSelector::set_color(Selector_Color color) {
    fs.state = static_cast<uint8_t>(color);
}

void CardSelector::deactivate() {
    target = nullptr;
    move(BJ_DEFAULT_SELECTOR_POS, 1.0);
    set_color(Selector_Color::GREY);
}

// nothing for now
void CardSelector::shake_target() {
    target->set_shake(BJ_SHAKE_DENY);
}

Card * CardSelector::get_target() {
    return target;
}

void CardSelector::remove_target() {
    target = nullptr;
}