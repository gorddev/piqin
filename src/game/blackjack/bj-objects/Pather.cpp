#include "../../../../include/game/blackjack/bj-objects/Pather.hpp"

#include "engine/particles/ParticleRhombus.hpp"
#include "../../../../include/game/blackjack/bj-objects/Deck.hpp"
//#include "engine/particles/ParticleCircle.hpp"
//#include "engine/particles/ParticleRect.hpp"
#include "engine/particles/ParticleSparkle.hpp"
#include "game/blackjack/Board.hpp"

#define ADDEV(t, f) gengine::GENG_Events.add_event(t, [=]{f;})

namespace blackjack {
    class Board;
}

using gengine::Vertex;
using namespace blackjack;

/* ........... */
/* Update*/
/* ........... */

void Pather::update(Deck& d) {
    for (auto it = travel_to_deck.begin(); it != travel_to_deck.end(); ) {
        if ((*it)->get_path() == nullptr) {
            d.add_card((*it));
            it = travel_to_deck.erase(it);
        }
        else
            ++it;
    }
}


/* ........... */
/* Deck*/
/* ........... */
void Pather::move(Card* c, Deck* deck) {
    if (c == nullptr) return;
    Vertex target = BJ_DEFAULT_DECK_POS
        + gengine::Vertex(deck->stack_height() + 1, deck->stack_height()-2, DECK_Z_BASE);
    c->set_path(target, gengine::GENG_Path::REBOUND, 0.25, true);
    c->shadow = false;
    c->flip_down();
    travel_to_deck.push_back(c);
}

void Pather::move(std::vector<Card*> cards, Deck* deck) {
    for (auto& c : cards) {
        ADDEV(20, pather.move(c, deck));
    }
}
/* ........... */
/* Doscard */
/* ........... */
// Legacy compatability function
void Pather::to_discard(Card* c, Discard* discard, short cardNum) {
    to_discard(c, discard, gengine::GENG_Path::BALLOON, cardNum);
}
// Regular function
void Pather::to_discard(Card * c, Discard* discard, gengine::GENG_Path path, short cardNum) {
    if (c == nullptr) return;
    c->shadow = false;
    c->set_z(DECK_Z_BASE + ((discard->size())/MAX_DECK_SIZE));
    c->set_path(BJ_DEFAULT_DISCARD_POS, path, 0.5);
    discard->add_card(c);
    c = nullptr;
}

void Pather::to_discard(std::vector<Card*>& cards, Discard* discard) {
    for (int i = 0; i < cards.size(); i++)
        ADDEV(std::max(abs(60 - 5*i), 10), to_discard(cards[i], discard, i));
}

/* ........... */
/* Hand */
/* ........... */

static gengine::Path hand_path(Hand& hand, Card*& c, const float& center, const float& height, int index = -1) {
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
        gengine::GENG_Path::BALLOON,
        0.5
    };
}

bool Pather::move(Card *c, Hand &hand) {
    if (c == nullptr) return false;
    // Path changes depending on if flayed or not.
    float center = gengine::glb::scene.width/2;
    float height = 9.8*gengine::glb::scene.height/10;
    if (!hand.add_card(c))
        return false;
    c->set_path(hand_path(hand, c, center, height), true);
   return true;
}

void Pather::update_hand(Hand& hand) {
    float center = gengine::glb::scene.width/2;
    float height = 9.8*gengine::glb::scene.height/10;
    std::vector<Card*> cards = hand.gather_objects();
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
    gengine::Vertex target(draw.get_pos().x - gengine::glb::scene.width/8.0f + index*24.0f, // NOLINT(*-narrowing-conversions)
        draw.get_pos().y - random()%8, DRAW_Z_BASE + (index/10.f));
    // EFIEOF
    c->set_z(DRAW_Z_BASE + (index/10.f));
    gengine::Path p = {
        target,
        c->pos(),
        gengine::GENG_Path::BALLOON,
        0.5
    };
    c->set_path(p);

}

void Pather::move(std::vector<Card*>& cards, Draw& draw) {
    for (int i = 0; i < cards.size(); i++)
        move(cards[i], draw, i);
}

void Pather::to_floater(Card *c) {
    if (c == nullptr) return;
    c->hidden = false;
    c->set_path(BJ_DEFAULT_FLOATER_POS, gengine::GENG_Path::BALLOON, 1.0);
    c->set_z(BJ_DEFAULT_FLOATER_POS.z);
    c->set_shake(BJ_SHAKE_FLOATER);
    auto* pr = new gengine::ParticleRhombus(c, 13.0, 0.7, -1, 200);
    bob.attach_new_particle(c, pr);
}

