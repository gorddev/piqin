#pragma once
#include "engine/layers/Layer.hpp"

namespace init {

    void init_card_layer(gan::Layer& layer);
    void set_card_layer_defaults(gan::Layer& layer);
    void add_card_frametables(gan::Layer& layer);
}
