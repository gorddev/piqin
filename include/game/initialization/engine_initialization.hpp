#pragma once

#include "engine/core/Engine.hpp"

namespace init {
    void init_engine(gan::Engine& engine, bool debug = false);

    void init_fonts(gan::Engine& engine);

    void init_routers(gan::Engine& engine);
}