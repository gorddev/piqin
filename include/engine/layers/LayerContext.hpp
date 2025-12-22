#pragma once
#include <string>
#include <vector>

#include "LayerMap.hpp"
#include "LayerState.hpp"
#include "engine/EngineContext.hpp"
#include "../debug/debug-utilities/Log.hpp"
#include "engine/rendering/Camera.hpp"

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

    constexpr bool operator<<(LayerFlag f1, LayerFlag f2);
    constexpr LayerFlag operator|(LayerFlag a, LayerFlag b);
    constexpr LayerFlag operator&(LayerFlag a, LayerFlag b);
    constexpr LayerFlag& operator|=(LayerFlag& a, LayerFlag b);
    constexpr LayerFlag& operator&=(LayerFlag& a, LayerFlag b);
    constexpr LayerFlag operator~(LayerFlag f) noexcept;

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
        /// State of the layer (time, keys held, ect)
        LayerState state;
        /// Camera of the layer
        Camera camera;
        /// Id of the layer
        int id = -1;

        /// Contains the engine Context
        EngineContext* world = nullptr;
    public:
        /// Default constructor.
        explicit LayerContext(std::string name);

        /// Returns the name of the layer
        [[nodiscard]] std::string get_name() const;

        /// Sets a new active layer
        void set_active_layer(std::string name);

        // *********************************
        // <><><> BoilerPlate Below! <><><>
        // *********************************
        // <><><> Enum Handlers <><><>
        // Visibility
        [[nodiscard]] bool is_visible() const;
        void visible();
        void hide();

        // Active
        [[nodiscard]] bool is_active() const;

        // Running
        [[nodiscard]] bool is_running() const;
        void run();
        void pause();

        // Initialized
        [[nodiscard]] bool is_initialized() const;

        // *****************
        // Z-ORDERING
        //******************
        // Strict
        [[nodiscard]] bool is_strict();
        void _strict();

        // Z-Ordered
        [[nodiscard]] bool is_z_indexed() const;
        void _z_index();

        // Batched
        [[nodiscard]] bool is_batched() const;
        void _batch();

        // <><><> Engine-Handles <><><>
        void _engine_flag(LayerFlag flag);
        void _engine_deflagger(LayerFlag flag);
        void _set_layer_map(LayerMap* l);
        void _update(double delta_time);
        void _add_engine_context(EngineContext* e);

        // <><><> Logging/Debug <><><>
        void log(debug::Log l);
        void log(int severity, std::string msg, std::string src = "");

        void log(int severity, std::stringstream msg,std::stringstream src);

        bool is_debug();

        // <><><> Returns the center of the scene <><><>
    };
}
