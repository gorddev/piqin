#pragma once
#include "Card-Selection-Defaults.hpp"
#include "engine/mods/animation/sprites/Sprite.hpp"

namespace card {

    struct CardHighlighter : gan::Sprite {
        /// local fader for the card highlighter
        gutils::Fader fader;
        /// permission to fade out
        bool permission_to_fade_in = false;

        /// Constructor for a card highlighter
        explicit CardHighlighter(gan::FrameTable& frame_table)
            : Sprite(frame_table, {{0,0}, 26, 36,1}, card_highlighter_anim_id),
            fader(1.4, {true, 0.8f})
        {
            hide();
            unset_shadow();
        }

        /// Updates dimensions according to a given card's transform
        void snap_to_card(gan::Transform2D& ct) {
            t.scale = ct.scale;
            if (t.pos.x == -1)
                t.pos = ct.pos;
            else if (t.pos.x != ct.pos.x)
                fader.fade_out();
            else if (permission_to_fade_in)
                fader.fade_in();
            t.pos = ct.pos;
            t.color.a = fader.calc_alpha(gan::max_alpha);
        }
    };
}
