#include "game/initialization/engine_initialization.hpp"
#include "engine/core/Engine.hpp"
#include "src/testing/camerarouter.hpp"

void init::init_engine(gan::Engine &engine, bool debug) {
    // initializes the engine
    engine.init(debug);
    // initializes fonts
    init_fonts(engine);
    init_routers(engine);
    // sets debut mode to true
    engine.set_debug_mode(true);
}


void init::init_fonts(gan::Engine& engine) {
    // Loads main pixel font
    engine.create_font("../assets/fonts/ka1.ttf", 1, 1);
    // Loads alternative clean font
    engine.create_font("../assets/fonts/RetroByte.ttf", 1, 1);
}

void init::init_routers(gan::Engine& engine) {
    // allows us to move camera with arrow keys
    engine.create_router<CameraRouter>();
}