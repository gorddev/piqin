#pragma once

#include <vector>

#include "Selector.hpp"

namespace gengine {
    struct Selection {
        short row;
        short column;
    };

    template <typename Item>
    class MenuRegion {
        // Takes a vector of vector pointers to
        std::vector<std::vector<Item*>*> menus;
        // Keeps track of the menus we have access to.
        std::vector<short> rows;
        // Tells us whether we wrap or not
        bool wrapy;
        bool wrapx;
        bool moveUp = true;
        // Lets us update a selector vertically with logic
        bool switch_selector_vertical();
        bool switch_selector_horizontal();
    public:
        // Keeps track of what we're actually selecting
        Selection s{};
        // Gives us a selector to move around
        Selector<Item>* selector;

    public:
        // Constructor
        MenuRegion(std::vector<std::vector<Item*>*> menus, bool wrapy, bool wrapx);

        // Lets us set our selectable regions
        void set_selectables(std::vector<short> selects);
        // Lets us add a selectable region
        void add_selectable(short s);

        // Now we can start coding our logic for movement
        // Directional Movement
        bool move_up();
        bool move_down();
        bool move_left();
        bool move_right();

        // Tells us to update our selector if something changed
        void update_selector(bool backByOne = false);
        // Lets us grab the row our selector is on
        int get_row();
        // Lets us grab the column our selector is on
        int get_column();
        // Lets us select the selector
        void set_selector(Selector<Item>* sel);
        // Lets us remove the selector
        void remove_selector();

        // Tostring solution
        string to_string();
    };
}
#include "src/engine/menu/MenuRegion.hpp"
