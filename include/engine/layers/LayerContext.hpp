#pragma once

#include "LayerFlag.hpp"
#include "LayerState.hpp"
#include "engine/core/EngineContext.hpp"
#include "engine/rendering/Camera.hpp"
#include "engine/types/external/vector.hpp"

namespace gan {

    /** @brief A LayerContext Object contains all the information needed to operate a layer under specific conditions.
     * These conditions include:
     * - @code double dt@endcode › The current relative change in time of the Layer.
     * - @code float speed@endcode › The speed of the layer and how fast events/objects move
     * - @code LayerFlag lflag@endcode › Flags the engine and layer use to determine display and management of the layer.
     */
    class LayerContext {
    public:
        /// Camera of the layer
        const Camera& camera;
    private:
        /// Name of the layer
        const fstring<10>& name;

        /// Id of the layer
        int id = -1;
        /// Keeps track of the state of the Layer. Default is visible & batched
        LayerFlag lflag = LayerFlag::visible | LayerFlag::batched | LayerFlag::running;
        /// Allows us to keep track of logs before the engine core is formed
        gch::vector<debug::Log> logs;

        /// Reference to scene width and height
        const uint16_t& canvas_width;
        const uint16_t& canvas_height;

        friend class LayerManager;
        friend class Renderer;
    public:
        /// State of the layer (time, keys held, ect)
        LayerState state;

    public:
        /// Default constructor.
        explicit LayerContext(const fstring<10>& name, const Camera& camera, const uint16_t& canvas_width, const uint16_t& canvas_height);

        /// Returns the name of the layer
        [[nodiscard]] fstring<10> get_name() const;

        /// Gets the camera of the layer
        [[nodiscard]] const Camera& get_camera() const;

        /// Gets the id of the layer
        [[nodiscard]] int get_id() const;

        // *********************************
        // <><><> BoilerPlate Below! <><><>
        // *********************************
        // <><><> Enum Handlers <><><>
        // Visibility
        [[nodiscard]] bool is_visible() const;
        void visible();

        void toggle_visible();

        void hide();

        // Active
        [[nodiscard]] bool is_active() const;

        // Running
        [[nodiscard]] bool is_running() const;
        void run();
        void pause();

        void toggle_running();

        // Initialized
        [[nodiscard]] bool is_initialized() const;

        // *****************
        // Z-ORDERING
        //******************
        // Strict
        [[nodiscard]] bool is_strict() const;
        void _strict();

        // Z-Ordered
        [[nodiscard]] bool is_z_indexed() const;
        void _z_index();

        // Batched
        [[nodiscard]] bool is_batched() const;
        void _batch();

        // <><><> Engine-Handles <><><>
        void _engine_flag(LayerFlag flag);
        void _engine_deflag(LayerFlag flag);
        void _update(double delta_time);

        // <><><> Logging/Debug <><><>
        void log(debug::Log l) const;
        void log(int severity, const char msg[], const char src[]);

        bool is_debug() const;
    };
}
