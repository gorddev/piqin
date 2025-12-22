#pragma once
#include <vector>

#include "Mouse.hpp"
#include "engine/types/Gear.hpp"
#include <unordered_map>

#include "engine/rendering/Camera.hpp"

namespace geng {

    /** @brief InputHandler is intended to be used exclusively by Layers to help manage the organization of Gears that accept inputs.
     * @details Input handler has two vectors, that contain the objects that accept keyboard input and those that accept mouse input.
     * It also has an active status, and a map containing keys that are currently held down. It has the following member functions
     */
    class InputHandler {
    private:
        /// Gears that can accept keyboard presses.
        std::vector<Gear*> key_press_acceptors;
        /// Gears that accept all held keys per frame.
        std::vector<Gear*> key_hold_acceptors;
        /// Gears that can accept mouse input.
        std::vector<Gear*> mouse_acceptors;

        /// Currently held keys
        std::unordered_map<SDL_Scancode, bool> heldKeys;
        /// Allows/Prevents input from going through
        bool active = true;
        /// Keeps track of the engine_context
        LayerContext& scene;
        /// Keeps track of the layer's camera
        Camera& cam;
    public:
        /// Keeps track of the mouse and it's target objects
        Mouse mouse;

        /// Default constructor
        explicit InputHandler(LayerContext& scene);

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
        /// Gets each frame and updates the state if we don't get input
        void _keys_down(uint8_t* keys) const;

        // ************************************
        //<><><> Adding/Removing Objects <><><>
        // ************************************
        /// Designates one object as a keyboard press input acceptor, such that it's function is called upon key press.
        void add_key_press_acceptor(Gear* g);
        /// Designates multiple objects as keyboard press acceptors
        void add_key_press_acceptors(std::vector<Gear*>& gears);
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

    };
}
