#pragma once
#include <vector>

#include "Mouse.hpp"
#include "engine/types/Gear.hpp"
#include <unordered_map>

namespace geng {

    /** @brief InputHandler is intended to be used exclusively by Layers to help manage the organization of Gears that accept inputs.
     * @details Input handler has two vectors, that contain the objects that accept keyboard input and those that accept mouse input.
     * It also has an active status, and a map containing keys that are currently held down. It has the following member functions
     */
    class InputHandler {
    private:
        /// Gears that can accept keyboard input.
        std::vector<Gear*> key_acceptors;
        /// Gears that can accept mouse input.
        std::vector<Gear*> mouse_acceptors;

        /// Currently held keys
        std::unordered_map<SDL_Scancode, bool> heldKeys;

        /// Allows/Prevents input from going through
        bool active = true;

        /// Keeps track of the world
        EngineContext* world = nullptr;
    public:
        /// Keeps track of the mouse and it's target objects
        Mouse mouse = Mouse(mouse_acceptors);

        /// Default constructor
        InputHandler() = default;

        // ************************************
        //<><><> Accepting Input <><><>
        // ************************************
        /// Called when key is pressed
        void _key_press(SDL_Scancode key);
        /// Called when a key is released
        void _key_release(SDL_Scancode key);
        /// Gets when a mouse is is_clicked
        void _mouse_click();
        /// Gets when a mouse click is released
        void _mouse_release();
        /// Gets when the mouse moves positions
        void _mouse_move(SDL_Point pos, float dx, float dy);

        // ************************************
        //<><><> Adding/Removing Objects <><><>
        // ************************************
        /// Designates one object as a keyboard input acceptor
        void add_key_acceptor(Gear* g);
        /// Designates multiple objects as keyboard acceptors
        void add_key_acceptors(std::vector<Gear*>& gears);
        /// Designates one object as a mouse_acceptor
        void add_mouse_acceptor(Gear* g);
        /// Designates multiple objects as mouse_acceptors
        void add_mouse_acceptors(std::vector<Gear*>& gears);

        /// Removes the key acceptor from the input handler
        void remove_key_acceptor(Gear* g);
        /// Removes multiple key acceptors from the input handler
        void remove_key_acceptors(std::vector<Gear*>& gears);
        /// Removes one gear from being a mouse acceptor
        void remove_mouse_acceptor(Gear* g);
        /// Removes multiple gears from being a mouse acceptor
        void remove_mouse_acceptors(std::vector<Gear*>& g);

        /// Removes all acceptors from the inputHandler
        void flush();
        /// Removes all keyboard acceptors from the inputHandler
        void flush_keyboard_acceptors();
        /// Removes all mouse acceptors from the InputHandler
        void flush_mouse_acceptors();

        /// Removes input attachments between the mouse and any target
        void refresh();
        /// Pauses input temporarily
        void pause();
        /// Resumes input if paused
        void resume();

        /// Adds the engine context to the input
        void _add_engine_context(EngineContext* ctx);

    };
}
