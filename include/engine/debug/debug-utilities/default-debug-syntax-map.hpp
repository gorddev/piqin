#pragma once
#include "engine/core/defaults/GengColors.hpp"
#include "engine/mods/banners/text/syntax/SyntaxMap.hpp"

namespace gan::debug {

    // Commands
    inline void syntax_emphasize(std::vector<str_subview> args, SyntaxInfo& out)
        { out.color = color_orange; }
    inline void syntax_normal(std::vector<str_subview> args, SyntaxInfo& out)
        { out.color = color_white; }
    inline void syntax_type(std::vector<str_subview> args, SyntaxInfo& out)
        { out.color = color_pink; }

    // Patterns
    void syntax_integers(str_view str, int pos, SyntaxInfo& out);

    // Highlights
    inline void syntax_true(SyntaxInfo& out) { out.color = color_green; }
    inline void syntax_false(SyntaxInfo& out) { out.color = color_red; }
    inline void syntax_nullptr(SyntaxInfo& out) { out.color = color_purple; }
    inline void syntax_ptr(SyntaxInfo& out) { out.color = color_light_blue; }

    const SyntaxMap gan_default_debug_syntax_map {
        {
            {"[e]", syntax_emphasize},
            {"[t]", syntax_type},
            {"[n]", syntax_normal}
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
        if (c == '.' && pos < str.length())
            c = str[pos + 1];
        if (c >= '0' && c <= '9')
            out.color = color_light_blue;
    }

}
