#include "../../../../include/game/blackjack/bj-objects/CardSelector.hpp"

#include <iostream>
#include <ostream>

#include "engine/pathing/path-types/BalloonPath.hpp"
#include "game/blackjack/BJEnums.hpp"

using namespace blackjack;

CardSelector::CardSelector() {
    target = nullptr;
    t = card_selector_transform;
    anim.set_frame_table_id(ASSET_CARD_SELECTOR_ID);
}

#define PATH_IT(t, off) bob.set_path( new geng::BalloonPath( &t, t.pos + off, 0.7))

// Changes cards. Default pathing = path true
void CardSelector::switchTarget(Card *newTarget, bool moveTarget) {
    // If our new target is a nullptr, we just deactivate
    if (newTarget == nullptr) {
        target = nullptr;
        deactivate();
        return;
    }
    // Otherwise we apply a path to the selector's target.
    PATH_IT(target->t, BJ_SELECT_OFFSET);

    target = newTarget;
    if (moveTarget)
        PATH_IT(newTarget->t, BJ_SELECT_OFFSET * -1);
    move(moveTarget);
}

// DOES NOT UPDATE A CARD'S PATH, JUST SELECTOR
void CardSelector::move(bool recess) {
    geng::FPos2D os = (recess) ? BJ_SELECTOR_OFFSET : BJ_SELECTOR_OFFSET * -1;
    PATH_IT(target->t, os);
}

// DOES NOT UPDATE A CARD'S PATH, JUST SELECTOR
void CardSelector::move(geng::FPos2D pos, float speed) {
    t.pos.z = SELECTOR_Z_BASE;
    bob.set_path(new geng::BalloonPath(&t, pos, speed));
    geng::FPos2D trueTarget = pos;
}

void CardSelector::set_color(Selector_Color color) {
    anim.set_animation(static_cast<uint8_t>(color));
}

void CardSelector::deactivate() {
    target = nullptr;
    move(BJ_DEFAULT_SELECTOR_POS, 1.0);
    set_color(Selector_Color::GREY);
}

// nothing for now
void CardSelector::shake_target() {
    bob.apply_effect(
        *target,
        new geng::EffectShake(geng::GENG_Shake::SIDE, 3.0, 300, 3.0f, true)
    );
}

Card * CardSelector::get_target() {
    return target;
}

void CardSelector::remove_target() {
    target = nullptr;
}