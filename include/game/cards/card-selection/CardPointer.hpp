#pragma once
#include "Card-Selection-Defaults.hpp"
#include "engine/mods/animation/sprites/Sprite.hpp"
#include "engine/utilities/Fader.hpp"

namespace card {

    struct CardPointer : gan::Sprite {
        /// Fader for the card pointer
        gutils::Fader fader;
        /// Determines whether we route the pointer to the next guy or not
        bool route_to_next_card = false;
        /// constructor for CardPointer
        explicit CardPointer(gan::FrameTable& frame_table)
            : Sprite(frame_table, {{0,0}, 16, 16}, card_pointer_anim_id),
            fader(1, false)
        {
            // We want the down pointer
            anim.set_frame(frame_table.get_frame(card_pointer_anim_id, 1));
            // set the shadow to false
            unset_shadow();
        }

        /// snaps the card pointer to the card
        void snap_to_card(gan::Transform2D& ct) {
            update_alpha();
        }

        /// updates the card's alpha according to the fader alpha
        void update_alpha() {
            t.color.a = fader.calc_alpha(gan::max_alpha);
        }

        /// Empty to_vertex function
        void to_vertex(gan::RenderBuffer &buffer) override {}

        /// To_vertex internal
        void to_vertex_internal(gan::RenderBuffer & buffer) {
            Sprite::to_vertex(buffer);
        }
    };
}
