#include "game/blackjack/Pather.hpp"

#include "engine/particles/ParticleRhombus.hpp"
#include "game/blackjack/cards/Deck.hpp"

using gengine::Vertex;
using namespace blackjack;

/* ........... */
/* Update*/
/* ........... */

void Pather::update(Deck& d) {
    for (auto& c : travel_to_deck) {
        if (c->get_path() == nullptr)
            d.add_card(c);
    }
}

/* ........... */
/* Deck*/
/* ........... */
void Pather::move(Card*& c, Deck& deck) {
    if (c == nullptr) return;
    Vertex target = BJ_DEFAULT_DECK_POS
        + gengine::Vertex(deck.stack_height(), deck.height()+4, DECK_Z_BASE);
    c->set_path(target, gengine::GENG_Path::SINE, 0.25, true);
    c->shadow = false;
    c->flip_down();
    travel_to_deck.push_back(c);
}

void Pather::move(std::vector<Card*>& cards, Deck& deck) {
    for (auto& c : cards)
        move(c, deck);
}

/* ........... */
/* Doscard */
/* ........... */
void Pather::to_discard(Card*& c, Discard& discard, short cardNum) {
    if (c == nullptr) return;
    gengine::Vertex unit;
    bob.remove_attached_particle(c);
    unit.randomize(20);
    c->shadow = false;
    c->set_z(DECK_Z_BASE + ((discard.size())/MAX_DECK_SIZE));
    c->set_shake(gengine::GENG_Shake::RANDOM, 1.0, (20*cardNum) + 100 + random()%300, 3, false);
    c->set_path(BJ_DEFAULT_DISCARD_POS+unit, gengine::GENG_Path::BALLOON, 0.5);
    discard.add_card(c);
    c = nullptr;
}

void Pather::move(std::vector<Card*>& cards, Discard& discard) {
    for (int i = 0; i < cards.size(); i++)
        to_discard(cards[i], discard, i);
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

bool Pather::move(Card *c, Hand &hand, int index) {
    if (c == nullptr) return false;
    // Path changes depending on if flayed or not.
    float center = gengine::glb::scene.width/2;
    float height = 9*gengine::glb::scene.height/10;
    if (!hand.add_card(c))
        return false;
    c->set_path(hand_path(hand, c, center, height), true);
   return true;
}

void Pather::update_hand(Hand& hand) {
    float center = gengine::glb::scene.width/2;
    float height = 9*gengine::glb::scene.height/10;
    std::vector<Card*> cards = hand.gather_objects();
    for (int i = 0; i < cards.size(); i++) {
        cards[i]->set_path(hand_path(hand, cards[i], center, height, i), true);
    }
}

/* ........... */
/* Draw */
/* ........... */

void Pather::move(Card *c, Draw& draw, int index) {
    if (c == nullptr) return;
    // Add to draw
    draw.add_card(c);
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
    c->set_shake(BJ_SHAKE_FLOATER);
    auto* pr = new gengine::ParticleRhombus(c, 13.0, 0.7, -1, 200);
    bob.attach_new_particle(c, pr);
}

