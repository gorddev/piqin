#include "game/cards/CardSelector.hpp"

CardSelector::CardSelector() {
    t = card_selector_transform;
    fs.sheet_id = ASSET_CARD_SELECTOR_ID;
}

void CardSelector::move(Vertex pos, double speed) {
    move(Path(pos, t.pos, PATH_BALLOON, speed));
}

void CardSelector::move(Path p) {
    set_path(p);
}

void CardSelector::move(Card* c, Vertex offset) {
    if (c->get_path() != nullptr)
        move(c->get_path()->get_target() + Vertex(0,5,0) + offset);
    else
        move(c->pos() + Vertex(0, 5, 0) + offset);
}

void CardSelector::set_color(uint8_t color) {
    if (color > 3) color = SELECTOR_COLOR_GREY;
    fs.state = color;
}

void CardSelector::deactivate(bool returnCard) {
    move(SELECTOR_DEFAULT_POS);
    set_color(SELECTOR_COLOR_GREY);
}
