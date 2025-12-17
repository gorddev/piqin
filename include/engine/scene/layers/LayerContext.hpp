#pragma once
#include <cstdint>
#include <string>
#include <utility>

#include "LayerMap.hpp"
#include "LayerTime.hpp"
#include "engine/EngineContext.hpp"
#include "engine/debug/Log.hpp"

namespace geng {
    /** @brief LayerFlags allows the engine and layer to behave differently based on flags set.
      * Flags avaliable are:
      * - @code visible@endcode
      * - @code active@endcode
      * - @code running@endcode
      * - @code initialized@endcode
      * - @code strict@endcode › If we have a strict rendering style in terms of order
      * - @code z_indexed@endcode › If rendering is z_indexed
      * - @code batched@endcode › If rendering is batched by texture (default)
      */
    enum class LayerFlag : uint8_t {
        none        = 0,
        visible     = 1 << 0,
        active      = 1 << 1,
        running     = 1 << 2,
        initialized = 1 << 3,
        strict      = 1 << 4,
        z_indexed   = 1 << 5,
        batched     = 1 << 6
    };

    constexpr bool operator<<(LayerFlag f1, LayerFlag f2) {
        return (static_cast<std::uint8_t>(f1) & static_cast<uint8_t>(f2));
    }

    constexpr LayerFlag operator|(LayerFlag a, LayerFlag b) {
        return static_cast<LayerFlag>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
    }

    constexpr LayerFlag operator&(LayerFlag a, LayerFlag b) {
        return static_cast<LayerFlag>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
    }

    constexpr LayerFlag& operator|=(LayerFlag& a, LayerFlag b) {
        a = a | b;
        return a;
    }
    constexpr LayerFlag& operator&=(LayerFlag& a, LayerFlag b) {
        a = a & b;
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
    constexpr LayerFlag operator~(LayerFlag f) noexcept {
        return static_cast<LayerFlag>(
            static_cast<uint8_t>(LayerFlagMask) &
            ~static_cast<uint8_t>(f)
        );
    }

    /** @brief A LayerContext Object contains all the information needed to operate a layer under specific conditions.
     * These conditions include:
     * - @code double dt@endcode › The current relative change in time of the Layer.
     * - @code float speed@endcode › The speed of the layer and how fast events/objects move
     * - @code LayerFlag lflag@endcode › Flags the engine and layer use to determine display and management of the layer.
     */
    class LayerContext {
    private:
        /// Keeps track of the state of the Layer. Default is visible & batched
        LayerFlag lflag = LayerFlag::visible | LayerFlag::batched;
        /// Name of the layer
        std::string name;
        /// Access to a layerMap to change layers
        LayerMap* layer_map = nullptr;
        /// Allows us to keep track of logs before the engine context is formed
        std::vector<debug::Log> logs;
    public:
        /// Time of the layer
        LayerTime time;
        /// Id of the layer
        int id = -1;

        /// Contains the engine Context
        EngineContext* world = nullptr;
    public:
        /// Default constructor.
        explicit LayerContext(std::string name) : name(std::move(name)) {}

        /// Returns the name of the layer
        [[nodiscard]] std::string get_name() const { return name; }

        /// Sets a new active layer
        void set_active_layer(std::string name) { layer_map->switch_layer(name);}

        // *********************************
        // <><><> BoilerPlate Below! <><><>
        // *********************************
        // <><><> Enum Handlers <><><>
        // Visibility
        /// Returns true if the Layer is visible
        [[nodiscard]] bool is_visible() const {
            return lflag << LayerFlag::visible;
        }
        /// Makes the Layer visible
        void visible() {
            lflag |= LayerFlag::visible;
        }
        /// Hides the layer
        void hide() {
            lflag &= ~LayerFlag::visible;
        }
        // Active
        /// Returns true if the Layer is the current active layer
        [[nodiscard]] bool is_active() const {
            return lflag << LayerFlag::active;
        }
        // Running
        /// Returns true if the layer is currently updating
        [[nodiscard]] bool is_running() const {
            return lflag << LayerFlag::running;
        }
        /// Starts running the layer's update loop
        void run() {
            lflag |= LayerFlag::running;
        }
        /// Pauses the layer's update loop.
        void pause() {
            lflag |= LayerFlag::running;
        }
        // Initialized
        [[nodiscard]] bool is_initialized() const {
            return lflag << LayerFlag::initialized;
        }
        // *****************
        // Z-ORDERING
        //******************
        // Strict
        /// Returns true if the ordering style is strict
        [[nodiscard]] bool is_strict() {
            return lflag << LayerFlag::strict;
        }
        void _strict() {
            lflag &= ~LayerFlag::batched;
            lflag &= ~LayerFlag::z_indexed;
            lflag |= LayerFlag::strict;
        }

        // Z-Ordered
        /// Returns true if the layer is z-indexed
        [[nodiscard]] bool is_z_indexed() const {
            return lflag << LayerFlag::z_indexed;
        }
        /// Sets the ordering style to z-ordered
        void _z_index() {
            lflag &= ~LayerFlag::batched;
            lflag &= ~LayerFlag::strict;
            lflag |= LayerFlag::z_indexed;
        }
        // Batched
        /// Returns true if the layer is batched
        [[nodiscard]] bool is_batched() const {
            return lflag << LayerFlag::batched;
        }
        /// Sets the ordering style to batched.
        void _batch() {
            lflag &= ~LayerFlag::z_indexed;
            lflag &= ~LayerFlag::strict;
            lflag |= LayerFlag::batched;
        }

        // <><><> Engine-Handles <><><>
        /// Called by the engine to flag the Layer
        void _engine_flag(LayerFlag flag);
        /// Called by the engine to deflag the Layer
        void _engine_deflagger(LayerFlag flag);
        /// Called by the engine to set up the layer map
        void _set_layer_map(LayerMap* l);
        /// Updates the LayerContext with relevant info
        void _update(double delta_time) { time._update(delta_time); }
        /// Adds the engine context to the LayerContext
        void _add_engine_context(EngineContext* e) {
            world = e;
        }
        // <><><> Logging/Debug <><><>
        /// Lets you log from the layer context
        void log(debug::Log l) {
            l.source = "ƒ:" + name + "/" + l.source;
            if (world != nullptr) {
                if (!logs.empty()) {
                    for (auto& l : logs)
                        world->log(l);
                    logs = {};
                }
                world->log(l);
            }
            else {
                l.message = "Tag: _Pre-Init_ " + l.message;
                std::cerr << l.to_string() << "\n";
                logs.push_back(l);
            }
        }
        void log(int severity, std::string msg, std::string src = "") {
            log(debug::Log(static_cast<debug::Severity>(severity), std::move(msg), std::move(src)));
        }
    };
}
