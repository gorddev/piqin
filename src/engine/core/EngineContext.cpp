#include "engine/core/EngineContext.hpp"

using namespace gan;

EngineContext::EngineContext() : tex_reg() {}

void EngineContext::update(double game_time) {
    time = game_time;
    dt = time - prevTime;
    prevTime = time;
    frame++;
}

void EngineContext::_set_window_size(uint16_t w, uint16_t h) {
    width = w;
    height = h;
}

void EngineContext::_set_border_size(uint16_t bx, uint16_t by) {
    borderX = bx;
    borderY = by;
}

void EngineContext::set_scale(float s) {
    scale = s;
}

void EngineContext::set_pixel_perfect(bool pix) {
    pixel_perfect = pix;
}

void EngineContext::enable_debug() {
    debugger.debug_mode = true;
}

void EngineContext::disable_debug() {
    debugger.debug_mode = false;
}

bool EngineContext::is_debug() const {
    return debugger.is_debug();
}

void EngineContext::set_layer_change(const gan::fstring<10>& new_layer) {
    layer_change = new_layer;
}

gan::fstring<10> EngineContext::get_layer_change() const {
    return layer_change;
}

double EngineContext::get_time() const {
    return time;
}

uint64_t EngineContext::get_frame() const {
    return frame;
}

double EngineContext::get_dt() const {
    return dt;
}

uint16_t EngineContext::get_width() const {
    return width;
}

uint16_t EngineContext::get_height() const {
    return height;
}

uint16_t EngineContext::get_borderX() const {
    return borderX;
}

uint16_t EngineContext::get_borderY() const {
    return borderY;
}

float EngineContext::get_scale() const {
    return scale;
}

bool EngineContext::is_pixel_perfect() const {
    return pixel_perfect;
}

Font& EngineContext::get_font(int id) {
    return fonts[id];
}
