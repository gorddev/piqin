#pragma once

#include "Selector.hpp"
#include "engine/types/external/vector.hpp"

namespace geng {
    struct Selection {
        short row;
        short column;
    };

    template <typename Item>
    class MenuRegion {
        // Takes a vector of vector pointers to
        gch::vector<gch::vector<Item*>*> menus;
        // Keeps track of the menus we have access to.
        gch::vector<short> rows;
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
        MenuRegion(gch::vector<gch::vector<Item*>*> menus, bool wrapy, bool wrapx);

        // Lets us set our selectable regions
        void set_selectables(gch::vector<short> selects);
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
        int row();
        // Lets us grab the column our selector is on
        int col();
        // Lets us select the selector
        void set_selector(Selector<Item>* sel);
        // Lets us remove the selector
        void remove_selector();

        // Tostring solution
        //std::string to_string();
    };
}

/* Implementation is below */

namespace geng {
    template<typename Item>
    MenuRegion<Item>::MenuRegion(gch::vector<gch::vector<Item*> *> menus, bool wrapx, bool wrapy)
        : menus(menus), wrapx(wrapx), s({0, 0}), selector(nullptr),
          wrapy(wrapy) {
    }

    template<typename Item>
    bool MenuRegion<Item>::move_up() {
        if (rows.size() <= 1) return false;

        if (s.row < rows.size() - 1) {
            bool found = false;
            for (size_t i = s.row + 1; i < rows.size(); ++i) {
                int actualRow = rows[i];
                if (actualRow >= 0 && actualRow < (int)menus.size() && menus[actualRow] && !menus[actualRow]->empty()) {
                    s.row = (int)i;
                    found = true;
                    break;
                }
            }
            if (!found) return false;
            return switch_selector_vertical();
        }

        if (wrapy) {
            s.row = 0;
            return switch_selector_vertical();
        }

        return false;
    }

    template<typename Item>
    bool MenuRegion<Item>::move_down() {
        if (rows.size() <= 1) return false;

        if (s.row > 0) {
            bool found = false;
            for (int i = s.row - 1; i >= 0; --i) {
                int actualRow = rows[i];
                if (actualRow >= 0 && actualRow < (int)menus.size() && menus[actualRow] && !menus[actualRow]->empty()) {
                    s.row = i;
                    found = true;
                    break;
                }
            }
            if (!found) return false;
            return switch_selector_vertical();
        }

        if (wrapy) {
            s.row = (int)rows.size() - 1;
            return switch_selector_vertical();
        }

        return false;
    }

    template<typename Item>
    bool MenuRegion<Item>::move_left() {
        if (rows.empty()) return false;
        int actualRow = rows[s.row];
        if (actualRow < 0 || actualRow >= (int)menus.size()) return false;
        auto *rowVec = menus[actualRow];
        if (!rowVec || rowVec->empty()) return false;

        if (s.column >= (int)rowVec->size())
            s.column = (int)rowVec->size() - 1;

        if (s.column > 0) {
            --s.column;
            return switch_selector_horizontal();
        }

        if (wrapx) {
            s.column = (int)rowVec->size() - 1;
            return switch_selector_horizontal();
        }

        return false;
    }

    template<typename Item>
    bool MenuRegion<Item>::move_right() {
        if (rows.empty()) return false;
        int actualRow = rows[s.row];
        if (actualRow < 0 || actualRow >= (int)menus.size()) return false;
        auto *rowVec = menus[actualRow];
        if (!rowVec || rowVec->empty()) return false;

        if (s.column >= (int)rowVec->size()) {
            s.column = 0;
            return switch_selector_horizontal();
        }

        if (s.column < (int)rowVec->size() - 1) {
            ++s.column;
            return switch_selector_horizontal();
        }

        if (wrapx) {
            s.column = 0;
            return switch_selector_horizontal();
        }

        return false;
    }

    template<typename Item>
    void MenuRegion<Item>::update_selector(bool backByOne) {
        if (rows.empty()) {
            if (selector) selector->deactivate();
            return;
        }
        if (s.row >= (int)rows.size()) s.row = (int)rows.size() - 1;
        int actualRow = rows[s.row];
        if (actualRow < 0 || actualRow >= (int)menus.size()) {
            if (selector) selector->deactivate();
            return;
        }
        auto &rowVec = *(menus[actualRow]);
        if (rowVec.empty()) {
            if (selector) selector->deactivate();
            return;
        }

        if (s.column >= (int)rowVec.size()) s.column = (int)rowVec.size() - 1;
        else if (backByOne && s.column > 0) --s.column;

        if (selector) selector->switchTarget(rowVec[s.column], moveUp);
    }

    template<typename Item>
    int MenuRegion<Item>::row() {
        if (rows.empty())
            return -1;
        if (s.row < 0)
            s.row = 0;
        if (s.row >= (int)rows.size())
            s.row = (int)rows.size() - 1;
        return rows[s.row];
    }

    template<typename Item>
    int MenuRegion<Item>::col() {
        return s.column;
    }

    template<typename Item>
    void MenuRegion<Item>::set_selector(Selector<Item> *sel) {
        selector = sel;
    }

    template<typename Item>
    void MenuRegion<Item>::remove_selector() {
        selector = nullptr;
    }

    /*
    template<typename Item>
    std::string MenuRegion<Item>::to_string() {
        std::string ret = "Selector: Row " + std::to_string(s.row) + ", Col " + std::to_string(s.column);
        ret += "\nNum Rows: " + std::to_string(rows.size()) + ", Num Columns: " + std::to_string(menus.size());
        ret += "\nPrinting Total Rows & Columns Avaliable: \n";
        for (auto& i: menus) {
            ret += "{";
            if (i) {
                for (auto& j: *i) {
                    ret+= "X, ";
                }
            }
            ret+=  "}\n";
        }
        ret += "Printing Avaliable rows:\n";
        ret += "{";
        for (auto& i: rows) {
            ret += std::to_string(i) + ", ";
        }
        ret+=  "}\n";
        return ret;
    }
    */

    template<typename Item>
    bool MenuRegion<Item>::switch_selector_vertical() {
        if (!selector || rows.empty()) return false;
        if (s.row < 0) s.row = 0;
        if (s.row >= (int)rows.size()) s.row = (int)rows.size() - 1;

        int actualRow = rows[s.row];
        if (actualRow < 0 || actualRow >= (int)menus.size()) return false;
        auto &menuRow = *(menus[actualRow]);
        if (menuRow.empty()) return false;

        if (s.column >= (int)menuRow.size()) s.column = (int)menuRow.size() - 1;
        selector->switchTarget(menuRow[s.column], moveUp);
        return true;
    }

    template<typename Item>
    bool MenuRegion<Item>::switch_selector_horizontal() {
        if (!selector || rows.empty()) return false;
        if (s.row < 0) s.row = 0;
        if (s.row >= (int)rows.size()) s.row = (int)rows.size() - 1;

        int actualRow = rows[s.row];
        if (actualRow < 0 || actualRow >= (int)menus.size()) return false;
        auto &menuRow = *(menus[actualRow]);
        if (menuRow.empty()) return false;

        selector->switchTarget(menuRow[s.column], moveUp);
        return true;
    }

    template<typename Item>
 void MenuRegion<Item>::set_selectables(gch::vector<short> selects) {
        // Update our rows
        rows = std::move(selects);

        if (selector != nullptr) {
            // If we have no selectable rows, deactivate and return
            if (rows.empty()) {
                selector->deactivate();
                return;
            }
            // Try to find the first row that actually has selectable items
            bool foundValid = false;
            for (int i = 0; i < rows.size(); ++i) {
                int actualRow = rows[i];
                // Check bounds and row content
                if (actualRow >= 0 &&
                    actualRow < (int)menus.size() &&
                    menus[actualRow] &&
                    !menus[actualRow]->empty()) {
                    s.row = i;   // found a valid row for the selector
                    foundValid = true;
                    break;
                    }
            }
            // If none of the selectable regions have any valid options,
            // deactivate the selector.
            if (!foundValid) {
                selector->deactivate();
                return;
            }
            // Otherwise update the selector normally
            update_selector();
        }
    }

    template<typename Item>
    void MenuRegion<Item>::add_selectable(short sel) {
        if (rows.empty()) {
            rows.push_back(sel);
            return;
        }
        bool inserted = false;
        for (size_t i = 0; i < rows.size(); ++i) {
            if (sel < rows[i]) {
                rows.insert(rows.begin() + i, sel);
                if (s.row >= (int)i) ++s.row;
                inserted = true;
                break;
            }
        }
        if (!inserted) rows.push_back(sel);
    }
}
