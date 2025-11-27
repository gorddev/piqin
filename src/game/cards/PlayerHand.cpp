#include "game/blackjack/PlayerHand.hpp"

#include <iostream>

#include "ObjectManager.hpp"
#include "input/InputConstants.hpp"

PlayerHand::PlayerHand() {
    // Set up our deal
    deal = new Deal(Vertex(3*scene::width/8,
        9*scene::height/16, 0));
    // Setting up the little selector
    selector = new CardSelector();
    flayed = false;
}

PlayerHand::~PlayerHand() {
    delete deal;
    delete selector;
}

bool PlayerHand::add_card_to_hand(Card* c) {
    if (cards.size() <= MAX_HAND_SIZE && c != nullptr) {
        c->set_z_index(HAND_Z_BASE+cards.size()/(MAX_HAND_SIZE*2.0f)); // NOLINT(*-narrowing-conversions)
        cards.push_back(c);
        update_cards(true);
    }
    else
        return false;
    return true;
}

Card* PlayerHand::pop_hand(uint8_t cardNum) {
    if (cardNum < cards.size()) {
        Card* ret = cards[cardNum];
        cards.erase(cards.begin()+cardNum);
        if (cards.size() != 0 && targetCard != 0)
            targetCard--;
        for (int i = targetCard; i < cards.size(); i++) {
            cards[i]->set_z_index(HAND_Z_BASE+i/(MAX_HAND_SIZE*2.0f));
        }
        update_cards(true);
        return ret;
    }
    return nullptr;
}

Card* PlayerHand::pop_hand() {
    return pop_hand(targetCard);
}

Path PlayerHand::get_card_path(int index, int& center, int& height, float speed, Vertex mod) {
    int flay = (is_held(INPUT_MODIFY)) ? (20) : 0;
    char target = (index == targetCard);
    Path p = {
        {
            // Space each card more x-wise if hand is flayed
            center + (14+flay) * (index - ((cards.size()-1)/2.0f)) + mod[0], // NOLINT(*-narrowing-conversions)
            // Set default height. If not flayed, stagger each card vertically from the last picked.
            (height - (flay*1.25))
                + ((!flayed) * abs(targetCard-index)*2)
                - ((flayed) * target *5) + mod[1], // NOLINT(*-narrowing-conversions)
            Z_HAND_BASE + index/(MAX_HAND_SIZE*2.0f) + mod[2] // NOLINT(*-narrowing-conversions)
        },
        cards[index]->pos(),
        PATH_BALLOON,
        speed
    };
    return p;
}

bool PlayerHand::selected_card_will_bust() {
    return deal->get_score() + selected_card()->get_score() > targetScore;
}

void PlayerHand::update_selector_color() {
    if (!flayed)
        selector->set_color(SELECTOR_COLOR_GREY);
    else if (selected_card_will_bust())
        selector->set_color(SELECTOR_COLOR_RED);
    else
        selector->set_color(SELECTOR_COLOR_YELLOW);

}

void PlayerHand::update_cards(bool force) {
    int center = scene::width/2;
    int height = 3*scene::height/4;

    if (cards.empty())
        selector->deactivate();
    else if (flayed != is_held(INPUT_MODIFY) || force) {
        flayed = is_held(INPUT_MODIFY);
        for (int i = 0; i < cards.size(); i++)
            cards[i]->set_path(get_card_path(i, center, height));
        // Dark or light
        if (!flayed) {
            selector->set_color(SELECTOR_COLOR_GREY);
        }

        Vertex offset = (flayed) ? Vertex(0,0,0) : Vertex(0,10,0);
        update_selector_color();
        selector->move(cards[targetCard], offset);
    }
    else if (targetCard != oldTarget) {
        selector->move(cards[targetCard], Vertex(0,-5,0));
        cards[targetCard]->set_path(get_card_path(targetCard, center, height));
        if (oldTarget < cards.size())
            cards[oldTarget]->set_path(get_card_path(oldTarget, center, height));
        oldTarget = targetCard;
        update_selector_color();
    }
}

bool PlayerHand::getPress(short keybind) {
    // Change the selector right
    if (keybind == INPUT_RIGHT && is_held(INPUT_MODIFY)) {
        targetCard = (targetCard + 1) % cards.size();
        return true;
    }
    // Change the selector left
    if (keybind == INPUT_LEFT&& is_held(INPUT_MODIFY)) {
        if ((targetCard - 1) < 0)
            targetCard = cards.size() - 1;
        else
            targetCard--;
        return true;
    }
    // Play a card
    if (keybind == INPUT_SELECT_ALT&& is_held(INPUT_MODIFY)) {
        play_card();
        return true;
    }
    // Take a card back
    if (keybind == INPUT_DOWN) {
        take_card();
        return true;
    }
    // Request a hit
    if (keybind == INPUT_SELECT) {
        hit = true;
        return true;
    }
    // Request a hit on the other person
    if (keybind == INPUT_BACK) {
        stay = true;
        return true;
    }

    return false;
}

Object* PlayerHand::get_hand_selector() const {
    return selector;
}

int PlayerHand::get_num_cards() const {
    return cards.size();
}

Deal * PlayerHand::get_deal() {
    return deal;
}

void PlayerHand::play_card() {
    if (selected_card_will_bust()) {
        selected_card()->set_shake(SHAKE_SIDE, 3, 300);
    }
    else
        deal->add_card(pop_hand());
}

void PlayerHand::take_card() {
    if (cards.size() != MAX_HAND_SIZE)
        add_card_to_hand(deal->pop_card());
}

Card* PlayerHand::selected_card() {
    return cards[targetCard];
}