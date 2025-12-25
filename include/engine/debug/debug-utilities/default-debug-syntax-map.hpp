#pragma once
#include "engine/defaults/GengColors.hpp"
#include "engine/scene/banners/text/syntax/SyntaxMap.hpp"

namespace geng::debug {

    // Commands
    inline void syntax_emphasize(gch::vector<str_subview> args, SyntaxInfo& out)
        { out.color = color_orange; }
    inline void syntax_unemphasize(gch::vector<str_subview> args, SyntaxInfo& out)
        { out.color = color_white; }

    // Patterns
    void syntax_integers(str_view str, int pos, SyntaxInfo& out);

    // Highlights
    inline void syntax_true(SyntaxInfo& out) { out.color = color_green; }
    inline void syntax_false(SyntaxInfo& out) { out.color = color_red; }
    inline void syntax_nullptr(SyntaxInfo& out) { out.color = color_blue; }
    inline void syntax_ptr(SyntaxInfo& out) { out.color = color_pink; }

    const SyntaxMap geng_default_debug_syntax_map {
        {
            {"[e]", syntax_emphasize},
            {"[n]", syntax_unemphasize}
        },
        {syntax_integers},
        {
            {"true", syntax_true},
            {"false", syntax_false},
            {"nullptr", syntax_nullptr},
            {"0x", syntax_ptr}
        }
    };

    inline void syntax_integers(str_view str, int pos, SyntaxInfo& out) {
        char c = str[pos];
        if (c >= '0' && c <= '9')
            out.color = color_pink;
        else if (c == '.' && pos < str.length() && (str[pos+1] >= '0' && str[pos+1] <= '9'))
            out.color = color_pink;
    }

}
