#include "game/initialization/card_game_initialization.hpp"
#include "game/initialization/initialization_defaults.hpp"
#include "game/initialization/initialization_data/small_card_frame_table.hpp"

void init::init_card_layer(gan::Layer &layer) {
    set_card_layer_defaults(layer);
    add_card_frametables(layer);
}


void init::set_card_layer_defaults(gan::Layer &layer) {
    layer.camera.set_dimensions({init::default_res_w, init::default_res_h});
}


void init::add_card_frametables(gan::Layer& layer) {
    layer.init.frame_table("../assets/cards/smallcard.png", small_card_table);
}