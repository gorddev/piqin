#pragma once
#include <cstdint>

namespace card {

    enum AffixType : uint16_t {
        mult = 0,
        add = 1
    };

    /// Additions to any particular card that can change its behavior
    struct Affix {
        /// Type of the particular affix
        AffixType affix_type;
        /// Value contianed within the affix
        int value;

        /// Displays the affix (optional)
        virtual void to_vertex() {}
        /// Updates the affix (optional)
        virtual int update() { return 0; }
        /// Modifies a card's specific score
        virtual void modify_score(long& running_score) { }
    };
}
