#pragma once

#include "engine/debug/DebugContext.hpp"
#include "engine/rendering/Camera.hpp"
#include "engine/types/strings/fstring/fstring.hpp"

#include <cstdint>

#include "engine/core/font/FontList.hpp"
#include "engine/scene/initializer/TextureRegister.hpp"

namespace gan {

    namespace debug { class Console; }

    /** The EngineContext's primary role is to give all members of the engine access
     *  to various utilities, such as a shadow database, and randomization
     */
    class EngineContext {
    private:
        /* ***************** */
        // Timing information //
        /* ***************** */
        /// The time of the previous render
        double prevTime = 0.0f;
        /// The current time
        double time = 0.0f;
        /// The current frame of the game
        uint64_t frame = 0;
        /// The current variation in time
        double dt = 0;

        /* ***************** */
        // Window information //
        /* ***************** */
        /// The width of the scene.
        uint16_t width = 200;
        /// The height of the scene.
        uint16_t height = 140;
        /// The size of one horizontal border on the screen
        uint16_t borderX = 0;
        /// The size of one vertical border on the screen
        uint16_t borderY = 0;
        /// The scale of the scene.
        float scale = 1.f;
        /// Whether we are pixel perfect or not
        bool pixel_perfect = false;
        /// Whether they request a layer change
        fstring<10> layer_change;

        /* ***************** */
        // Font information //
        /* ***************** */
        /// Contains a pointer to the font list
        FontList fonts;

        friend class Engine;
        friend class Layer;
        friend class debug::Console;
    public:
        /* ***************** */
        // Texture register //
        /* ***************** */
        /// Texture register for the engine
        TextureRegister tex_reg;

        /* ***************** */
        // Window information //
        /* ***************** */
        /// Lets us have debug utilities
        debug::DebugContext debugger;

        EngineContext();

        /// Updates the engineContext to get new timings and everything!!!
        void update(double game_time);

        /// Sets the window size of the current core. Not to be used except by the Engine.
        void _set_window_size(uint16_t w, uint16_t h);

        /// Sets the border size of the current core. Not to be used except by the engine
        void _set_border_size(uint16_t bx, uint16_t by);

        /// Sets the scale of the current scene
        void set_scale(float s);

        /// Getters
        [[nodiscard]] double get_time() const;
        [[nodiscard]] uint64_t get_frame() const;
        [[nodiscard]] double get_dt() const;
        [[nodiscard]] uint16_t get_width() const;
        [[nodiscard]] uint16_t get_height() const;
        [[nodiscard]] uint16_t get_borderX() const;
        [[nodiscard]] uint16_t get_borderY() const;
        [[nodiscard]] float get_scale() const;
        [[nodiscard]] bool is_pixel_perfect() const;
        [[nodiscard]] Font& get_font(int id);

        /// Pixel perfect settings
        void set_pixel_perfect(bool pix);

        void enable_debug();
        void disable_debug();
        [[nodiscard]] bool is_debug() const;

        void set_layer_change(const gan::fstring<10>& new_layer);
        [[nodiscard]] gan::fstring<10> get_layer_change() const;
    };

} // namespace gan