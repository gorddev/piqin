#pragma once
#include "board/Deck.hpp"
#include "cards/card-selection/CardSelector.hpp"
#include "engine/input/InputRouter.hpp"

namespace card {

    class GameMaster : public gan::InputRouter {
    private:
        /// Deck that contains all the cards
        Deck deck;
        /// Highlights the current card selection
        CardSelector card_selector;
        /// The layer the GameMaster is bound to
        gan::Layer& bound_layer;
    public:
        GameMaster(gan::InputRouterInit &init, gan::Layer& bound_layer)
            : InputRouter(init),
            deck(bound_layer.mod<gan::Anim>()->get_ft(0)),
            card_selector(bound_layer),
            bound_layer(bound_layer)
        {
            // Add all the objects from the deck
            for (auto& c: deck.gather_objects()) {
                bound_layer.mod<gan::Anim>()->add_sprite(c);
                bound_layer.input.add_mouse_acceptor(c);
            }
            bound_layer.mod<gan::Anim>()->add_sprite(&card_selector);
            // Hide the card highlighter.
            card_selector.hide();
            card_selector.t.scale = 1;
        }

        bool pre_update(gan::Layer *&active_layer, const uint8_t *keys) override {
            return false;
        }

        void post_update(gan::Layer *&active_layer, const uint8_t *keys) override {
            auto gear = active_layer->input.mouse.target;
            if (gear != nullptr) {
                if (gear->user_tag == 'c')
                    card_selector.on_hover_card(static_cast<Card*>(gear));
                else
                    card_selector.on_hover_release();
            }
            else
                card_selector.on_hover_release();

        }
    };
}
