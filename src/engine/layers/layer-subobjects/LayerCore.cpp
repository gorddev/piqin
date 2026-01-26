#include "../../../../include/engine/layers/layer-subobjects/LayerContext.hpp"
#include "engine/debug/geng_debug.hpp"

using namespace gan;


LayerContext::LayerContext(const fstring<10> &name, const Camera &camera, const uint16_t &canvas_width, const uint16_t &canvas_height)
        : camera(camera), name(name), canvas_width(canvas_width),canvas_height(canvas_height),
            state(lflag, canvas_width, canvas_height) {}


int LayerContext::get_id() const {
    return id;
}

bool LayerContext::is_visible() const { return static_cast<bool>(lflag & LayerFlag::visible); }
void LayerContext::visible() { lflag |= LayerFlag::visible; }
void LayerContext::toggle_visible() {if (is_visible()) hide(); else visible(); }
void LayerContext::hide() { lflag &= ~LayerFlag::visible; }

bool LayerContext::is_active() const { return static_cast<bool>(lflag & LayerFlag::active); }

bool LayerContext::is_running() const { return static_cast<bool>(lflag & LayerFlag::running); }
void LayerContext::run() { lflag |= LayerFlag::running; }
void LayerContext::pause() { lflag &= ~LayerFlag::running; }
void LayerContext::toggle_running() { if (is_running()) pause(); else run(); }

bool LayerContext::is_initialized() const { return static_cast<bool>(lflag & LayerFlag::initialized); }

bool LayerContext::is_strict() const { return static_cast<bool>(lflag & LayerFlag::strict); }
void LayerContext::_strict() {
    lflag &= ~LayerFlag::batched;
    lflag &= ~LayerFlag::z_indexed;
    lflag |= LayerFlag::strict;
}

bool LayerContext::is_z_indexed() const { return static_cast<bool>(lflag & LayerFlag::z_indexed); }
void LayerContext::_z_index() {
    lflag &= ~LayerFlag::batched;
    lflag &= ~LayerFlag::strict;
    lflag |= LayerFlag::z_indexed;
}

bool LayerContext::is_batched() const { return static_cast<bool>(lflag & LayerFlag::batched); }
void LayerContext::_batch() {
    lflag &= ~LayerFlag::z_indexed;
    lflag &= ~LayerFlag::strict;
    lflag |= LayerFlag::batched;
}

void LayerContext::_engine_flag(LayerFlag flag) {
    lflag |= flag;
}

void LayerContext::_engine_deflag(LayerFlag flag) {
    lflag &= ~flag;
}

void LayerContext::_update(double delta_time) { state._update(delta_time); }