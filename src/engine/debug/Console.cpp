#include "engine/debug/Console.hpp"

using namespace gan;
using namespace gan::debug;

bool Console::get_key_press(SDL_Scancode key, Layer *&active_layer) {

    if (key == SDL_SCANCODE_F1) {
        debugger.debug_mode = !debugger.debug_mode;
    }
    else if (key == SDL_SCANCODE_RETURN) {
        info_banner.get_press(key, active_layer);
    }
    else {
        input_handler._get_key_press(key);
    }

    return false;
}


bool Console::get_mouse_click(Uint8 button, Pos2D mousepos, Layer *&active_layer) {
    input_handler._get_mouse_click(mousepos);
    if (debugger.change_layer) {
        core.set_layer_change("__next");
        debugger.change_layer = false;
    }
    if (debugger.pause_layer) {
        core.set_layer_change("__run");
        debugger.pause_layer = false;
    }
    return false;
}

bool Console::get_mouse_release(Uint8 button, Pos2D mousepos, Layer *&active_layer) {
    input_handler._get_mouse_release(mousepos);
    return false;
}

bool Console::get_mouse_move(Pos2D mousepos, FPos2D deltapos, Layer *&active_layer) {
    input_handler._mouse_move(mousepos, deltapos);
    return false;
}



