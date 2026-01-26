#include "../../../../include/engine/layers/layer-subobjects/LayerCore.hpp"
#include "engine/debug/geng_debug.hpp"

using namespace gan;


LayerCore::LayerCore(const fstring<10> &name, const Camera &camera,
        const uint16_t &canvas_width, const uint16_t &canvas_height,
        GearRemovalFunction gear_init, void* module_manager, TextureRegister& tex_reg)
    : state(lflag, canvas_width, canvas_height),
        gears(gear_init, id, module_manager),
        camera(camera),
        canvas_width(canvas_width),
        canvas_height(canvas_height), name(name),
        tex_reg(tex_reg) {}


int LayerCore::get_id() const {
    return id;
}

bool LayerCore::is_visible() const { return static_cast<bool>(lflag & LayerFlag::visible); }
void LayerCore::visible() { lflag |= LayerFlag::visible; }
void LayerCore::toggle_visible() {if (is_visible()) hide(); else visible(); }
void LayerCore::hide() { lflag &= ~LayerFlag::visible; }

bool LayerCore::is_active() const { return static_cast<bool>(lflag & LayerFlag::active); }

bool LayerCore::is_running() const { return static_cast<bool>(lflag & LayerFlag::running); }
void LayerCore::run() { lflag |= LayerFlag::running; }
void LayerCore::pause() { lflag &= ~LayerFlag::running; }
void LayerCore::toggle_running() { if (is_running()) pause(); else run(); }

bool LayerCore::is_initialized() const { return static_cast<bool>(lflag & LayerFlag::initialized); }

bool LayerCore::is_strict() const { return static_cast<bool>(lflag & LayerFlag::strict); }
void LayerCore::_strict() {
    lflag &= ~LayerFlag::batched;
    lflag &= ~LayerFlag::z_indexed;
    lflag |= LayerFlag::strict;
}

bool LayerCore::is_z_indexed() const { return static_cast<bool>(lflag & LayerFlag::z_indexed); }
void LayerCore::_z_index() {
    lflag &= ~LayerFlag::batched;
    lflag &= ~LayerFlag::strict;
    lflag |= LayerFlag::z_indexed;
}

bool LayerCore::is_batched() const { return static_cast<bool>(lflag & LayerFlag::batched); }
void LayerCore::_batch() {
    lflag &= ~LayerFlag::z_indexed;
    lflag &= ~LayerFlag::strict;
    lflag |= LayerFlag::batched;
}

void LayerCore::_engine_flag(LayerFlag flag) {
    lflag |= flag;
}

void LayerCore::_engine_deflag(LayerFlag flag) {
    lflag &= ~flag;
}

void LayerCore::_update(double delta_time) { state._update(delta_time); }