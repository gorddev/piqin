#include "engine/layers/LayerContext.hpp"

#include <sstream>

using namespace geng;

constexpr bool geng::operator<<(LayerFlag f1, LayerFlag f2) {
    return (static_cast<std::uint8_t>(f1) & static_cast<uint8_t>(f2));
}

constexpr LayerFlag geng::operator|(LayerFlag a, LayerFlag b) {
    return static_cast<LayerFlag>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

constexpr LayerFlag geng::operator&(LayerFlag a, LayerFlag b) {
    return static_cast<LayerFlag>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
}

constexpr LayerFlag& geng::operator|=(LayerFlag& a, LayerFlag b) {
    a = static_cast<LayerFlag>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    return a;
}

constexpr LayerFlag& geng::operator&=(LayerFlag& a, LayerFlag b) {
    a = static_cast<LayerFlag>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
    return a;
}

constexpr LayerFlag LayerFlagMask =
    LayerFlag::visible     |
    LayerFlag::active      |
    LayerFlag::running     |
    LayerFlag::initialized |
    LayerFlag::strict      |
    LayerFlag::z_indexed   |
    LayerFlag::batched;

constexpr LayerFlag geng::operator~(LayerFlag f) noexcept {
    return static_cast<LayerFlag>(
        static_cast<uint8_t>(LayerFlagMask) &
        ~static_cast<uint8_t>(f)
    );
}

// LayerContext methods
LayerContext::LayerContext(std::string name) : name(std::move(name)) {
    std::cerr << "making hte layercontext\n";
}

std::string LayerContext::get_name() const { return name; }

void LayerContext::set_active_layer(std::string name) { layer_map->switch_layer(name); }

bool LayerContext::is_visible() const { return lflag << LayerFlag::visible; }
void LayerContext::visible() { lflag |= LayerFlag::visible; }
void LayerContext::hide() { lflag &= ~LayerFlag::visible; }

bool LayerContext::is_active() const { return lflag << LayerFlag::active; }

bool LayerContext::is_running() const { return lflag << LayerFlag::running; }
void LayerContext::run() { lflag |= LayerFlag::running; }
void LayerContext::pause() { lflag &= ~LayerFlag::running; }

bool LayerContext::is_initialized() const { return lflag << LayerFlag::initialized; }

bool LayerContext::is_strict() { return lflag << LayerFlag::strict; }
void LayerContext::_strict() {
    lflag &= ~LayerFlag::batched;
    lflag &= ~LayerFlag::z_indexed;
    lflag |= LayerFlag::strict;
}

bool LayerContext::is_z_indexed() const { return lflag << LayerFlag::z_indexed; }
void LayerContext::_z_index() {
    lflag &= ~LayerFlag::batched;
    lflag &= ~LayerFlag::strict;
    lflag |= LayerFlag::z_indexed;
}

bool LayerContext::is_batched() const { return lflag << LayerFlag::batched; }
void LayerContext::_batch() {
    lflag &= ~LayerFlag::z_indexed;
    lflag &= ~LayerFlag::strict;
    lflag |= LayerFlag::batched;
}

void LayerContext::_update(double delta_time) { state._update(delta_time); }

void LayerContext::_add_engine_context(EngineContext* e) { world = e; }

void LayerContext::log(debug::Log l) {
    l.source = "ƒ:" + name + "/" + l.source;
    if (world != nullptr) {
        if (!logs.empty()) {
            for (auto& l : logs)
                world->log(l);
            logs = {};
        }
        world->log(l);
    } else {
        l.message = "ƒ: " + name + "_Pre-Init_ " + l.message;
        std::cerr << l.to_string() << std::endl;
        logs.push_back(l);
    }
}

void LayerContext::log(int severity, std::string msg, std::string src) {
    log(debug::Log(static_cast<debug::Severity>(severity), std::move(msg), std::move(src)));
}



bool LayerContext::is_debug() {
    if (world != nullptr)
        return world->is_debug();
    return false;
}
