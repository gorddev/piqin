#include "../../../../include/game/blackjack/bj-objects/Pather.hpp"

#include "../../../../include/engine/particles/particle-types/ParticleRhombus.hpp"
#include "../../../../include/game/blackjack/bj-objects/Deck.hpp"
//#include "engine/particles/ParticleCircle.hpp"
//#include "engine/particles/ParticleRect.hpp"
#include "../../../../include/engine/particles/particle-types/ParticleSparkle.hpp"
#include "game/blackjack/Board.hpp"

#define ADDEV(t, f) geng::GENG_Events.add_event(t, [=]{f;})

namespace blackjack {
    class Board;
}

using geng::FPos2D;
using namespace blackjack;

/* ........... */
/* Update*/
/* ........... */

void Pather::update(Deck& d) { }

void Pather::move_card_to_deck(int id) {
    for (auto it = travel_to_deck.begin(); it != travel_to_deck.end(); it++) {
        if (id == (*it)->t.id) {
            Deck
            travel_to_deck.erase(it);
        }
    }
}


/* ........... */
/* Deck*/
/* ........... */
void Pather::move(Card* c, Deck* deck) {
    if (c == nullptr) return;
    FPos2D target = BJ_DEFAULT_DECK_POS
        + geng::FPos2D(deck->stack_height() + 1, deck->stack_height()-2, DECK_Z_BASE);
    c->set_path(target, geng::GENG_Path::REBOUND, 0.25, true);
    c->shadow = false;
    c->flip_down();
    travel_to_deck.push_back(c);
}

void Pather::move(lni::vector<Card*> cards, Deck* deck) {
    for (auto& c : cards) {
        ADDEV(20, pather.move(c, deck));
    }
}
/* ........... */
/* Doscard */
/* ........... */
// Legacy compatability function
void Pather::to_discard(Card* c, Discard* discard, short cardNum) {
    to_discard(c, discard, geng::GENG_Path::BALLOON, cardNum);
}
// Regular function
void Pather::to_discard(Card * c, Discard* discard, geng::GENG_Path path, short cardNum) {
    if (c == nullptr) return;
    c->shadow = false;
    c->set_z(DECK_Z_BASE + ((discard->size())/MAX_DECK_SIZE));
    c->set_path(BJ_DEFAULT_DISCARD_POS, path, 0.5);
    discard->add_card(c);
    c = nullptr;
}

void Pather::to_discard(lni::vector<Card*>& cards, Discard* discard) {
    for (int i = 0; i < cards.size(); i++)
        ADDEV(std::max(abs(60 - 5*i), 10), to_discard(cards[i], discard, i));
}

/* ........... */
/* Hand */
/* ........... */

static geng::Path hand_path(Hand& hand, Card*& c, const float& center, const float& height, int index = -1) {
    float flay = hand.flayed * 20;
    if (index == -1)
        index = hand.get_index(c);
    return {
        // Target
        {
            // X-Direction
            center + (14+flay) * (index - ((hand.size()-1)/2.0f)),
            // Y-direction
            (height - (flay*1.25f))
                    + ((!flay) * abs(hand.lastTarget-index)*2) // NOLINT(*-narrowing-conversions)
                    - (hand.flayed),
            // Z-diection
            HAND_Z_BASE + 1.0f - abs(hand.lastTarget-index)/(maxHandSize*2.0f)
        },
        c->pos(),
        geng::GENG_Path::BALLOON,
        0.5
    };
}

bool Pather::move(Card *c, Hand &hand) {
    if (c == nullptr) return false;
    // Path changes depending on if flayed or not.
    float center = geng::global::scene.width/2;
    float height = 9.8*geng::global::scene.height/10;
    if (!hand.add_card(c))
        return false;
    c->set_path(hand_path(hand, c, center, height), true);
   return true;
}

void Pather::update_hand(Hand& hand) {
    float center = geng::global::scene.width/2;
    float height = 9.8*geng::global::scene.height/10;
    lni::vector<Card*> cards = hand.gather_objects();
    for (int i = 0; i < cards.size(); i++) {
        cards[i]->set_path(hand_path(hand, cards[i], center, height, i), true);
    }
}

/* ........... */
/* Draw */
/* ........... */

void Pather::move(Card *c, Draw& draw, int index, bool down) {
    if (c == nullptr) return;
    // Add to draw
    draw.add_card(c);
    if (down)
        c->flip_down();
    c->flip_up();
    if (index == -1)
        index = draw.get_index(c);
    // Gets target location for the draw
    geng::FPos2D target(draw.get_pos().x - geng::global::scene.width/8.0f + index*24.0f, // NOLINT(*-narrowing-conversions)
        draw.get_pos().y - random()%8, DRAW_Z_BASE + (index/10.f));
    // EFIEOF
    c->set_z(DRAW_Z_BASE + (index/10.f));
    geng::Path p = {
        target,
        c->pos(),
        geng::GENG_Path::BALLOON,
        0.5
    };
    c->set_path(p);

}

void Pather::move(lni::vector<Card*>& cards, Draw& draw) {
    for (int i = 0; i < cards.size(); i++)
        move(cards[i], draw, i);
}

void Pather::to_floater(Card *c) {
    if (c == nullptr) return;
    c->t.hidden = false;
    c->set_path(BJ_DEFAULT_FLOATER_POS, geng::GENG_Path::BALLOON, 1.0);
    c->set_z(BJ_DEFAULT_FLOATER_POS.z);
    c->set_shake(BJ_SHAKE_FLOATER);
    auto* pr = new geng::ParticleRhombus(c, 13.0, 0.7, -1, 200);
    bob.attach_particle(c, pr);
}

