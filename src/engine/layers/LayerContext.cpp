#include "engine/layers/LayerContext.hpp"

#include <sstream>

#include "engine/debug/logging/LogSource.hpp"

using namespace geng;


// LayerContext methods
LayerContext::LayerContext(EngineContext &core, const geng::fstring<10>& name, const Camera& camera)
    : core(core), name(name), camera(camera), state(lflag) {
    glog::note.src("layercontext") << "making layer context." << glog::endlog;
}

geng::fstring<10> LayerContext::get_name() const { return name; }

const Camera & LayerContext::get_camera() const {
    return camera;
}

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

void LayerContext::log(debug::Log l) const {
    l.source << "ƒ:" << get_name().cstr() << "/";
    glog::note << l;
}

void LayerContext::log(int severity, const char msg[], const char src[]) {
    log(debug::Log(static_cast<debug::Severity>(severity), msg, src));
}

bool LayerContext::is_debug() const {
    return core.is_debug();
}
