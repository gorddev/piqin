#pragma once
#include "../GengColors.hpp"
#include "engine/scene/banners/text/syntax/SyntaxMap.hpp"
#include "engine/utilities/Utilities.hpp"

namespace gan {

    inline void default_scale_syntax_command(std::vector<str_subview> args, SyntaxInfo& out);
    inline void default_color_syntax_command(std::vector<str_subview> args, SyntaxInfo& out);
    inline void default_color_name_syntax_command(std::vector<str_subview> args, SyntaxInfo& out);
    inline void default_opacity_syntax_command(std::vector<str_subview> args, SyntaxInfo& out);

    inline void highlight_true(SyntaxInfo& out);

    const SyntaxMap gan_default_syntax_map {
        { // commands
            {"[s:@]", default_scale_syntax_command},
            {"[c:@]", default_color_name_syntax_command},
            {"[c:@,@,@]", default_color_syntax_command},
            {"[o:@]", default_opacity_syntax_command}
        },
        {}, // patterns
        {{"true", highlight_true}} // highlights
    };

    inline void default_scale_syntax_command(std::vector<str_subview> args, SyntaxInfo& out) {
        gutils::cstr_to_int(args[0].c_str(), out.scale);
    }

    inline void default_color_syntax_command(std::vector<str_subview> args, SyntaxInfo& out) {
        int r, g, b;
        gutils::cstr_to_int(args[0].c_str(), r);
        gutils::cstr_to_int(args[1].c_str(), g);
        gutils::cstr_to_int(args[2].c_str(), b);
        out.color.r = r;
        out.color.g = g;
        out.color.b = b;
    }

    inline void default_color_name_syntax_command(std::vector<str_subview> args, SyntaxInfo &out) {
        if (args[0] == "red") out.color =  color_red;
        else if (args[0] == "green")  out.color = color_green;
        else if (args[0] == "blue")   out.color =  color_blue;
        else if (args[0] == "orange") out.color =  color_orange;
        else if (args[0] == "yellow") out.color =  color_yellow;
        else if (args[0] == "pink") out.color = color_pink;
        else if (args[0] == "white")  out.color = color_white;
        else if (args[0] == "black")  out.color =  {0, 0, 0, gan::max_alpha};
        else out.color = {gan::max_alpha, gan::max_alpha, gan::max_alpha, gan::max_alpha};
    }

    inline void default_opacity_syntax_command(std::vector<str_subview> args, SyntaxInfo &out) {
        int a;
        gutils::cstr_to_int(args[0].c_str(), a);
        out.color.a = a;
    }

    inline void highlight_true(SyntaxInfo &out) {
        out.color = {0,gan::max_alpha,0,gan::max_alpha};
    }
}
