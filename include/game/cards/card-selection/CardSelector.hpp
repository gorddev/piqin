#pragma once
#include <engine/gan_plugins.hpp>

#include "CardHighlighter.hpp"
#include "CardPointer.hpp"
#include "../single-cards/Card.hpp"
#include "game/initialization/initialization_defaults.hpp"

namespace card {

    class CardSelector : public gan::Sprite {
    private:
        // Contains the connected layer
        gan::Layer& bound_layer;
        // Contains the highlighter of the card
        CardHighlighter highlighter;
        // Contains the pointer to the card
        CardPointer pointer;
        // Contains the attached card
        Card* payload = nullptr;

        // Make sure game master can access protected variabels
        friend class GameMaster;
    public:
        /// Constructor for a card highlighter
        explicit CardSelector(gan::Layer& card_layer)
            : Sprite(card_layer.mod<gan::Anim>()->get_ft(card_selector_frame_table_id)),
            bound_layer(card_layer),
            // highlights the current card
            highlighter(card_layer.mod<gan::Anim>()->get_ft(card_selector_frame_table_id)),
            // adds the pointer to the top of the card
            pointer(card_layer.mod<gan::Anim>()->get_ft(card_selector_frame_table_id))
        {
            hide();
            t.color.a = 0;
            // add sprites to the layer
            card_layer.mod<gan::Anim>()->add_sprite(&highlighter);
            card_layer.mod<gan::Anim>()->add_sprite(&pointer);


            // add morphs to the sprites
        }

        /// Updater for the card highlighter
        void update(gan::LayerState& time) override {
            // iterate through all of the current targets
            highlighter.fader.update(time.get_dt(), time.get_time());
            pointer.fader.update(time.get_dt(), time.get_time());
        }

        /// on hovering
        void on_hover_card(Card* target_card) {
            if (target_card!= payload || pointer.fader.fade_status == gutils::Fader::FADE_NONE) {
                if (pointer.fader.fade_status != gutils::Fader::FADE_MAX && pointer.fader.fade_status != gutils::Fader::FADE_IN) {
                    // update fader progress to nothing
                    // fades them in
                    highlighter.fader.fade_in();
                    pointer.fader.fade_in();
                    // sets z_index
                    highlighter.z_index = target_card->z_index-2;
                    pointer.z_index = target_card->z_index+2;
                    // fading permission
                    highlighter.permission_to_fade_in = true;
                }
                else
                    pointer.route_to_next_card = true;

                // set the payload
                payload = target_card;
            }
        }

        /// on hover release
        void on_hover_release() override {
            highlighter.fader.fade_out();
            highlighter.permission_to_fade_in = false;
            pointer.fader.fade_out();
        }

        /// to vertex
        void to_vertex(gan::RenderBuffer &buffer) override {
            // then update current position
            if (payload != nullptr) {
                highlighter.snap_to_card(payload->t);

                if (!payload->is_dragged())
                    bound_layer.mod<gfx::RouteModule>()->add_route(new gfx::Balloon(pointer, payload->t.pos + gan::vec2{0.f, -8.f*t.scale-1.f*16*t.scale}, 0.4));
                else
                    pointer.t.pos = payload->t.pos + gan::vec2{0.f, -8.f*t.scale-1.f*16*t.scale};
                pointer.snap_to_card(payload->t);
                pointer.route_to_next_card = false;
            }
            pointer.to_vertex_internal(buffer);

        }
    };
}
