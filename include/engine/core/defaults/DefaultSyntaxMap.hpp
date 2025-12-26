#pragma once
#include "GengColors.hpp"
#include "engine/scene/banners/text/syntax/SyntaxMap.hpp"
#include "engine/utilities/Utilities.hpp"

namespace geng {

    inline void default_scale_syntax_command(gch::vector<str_subview> args, SyntaxInfo& out);
    inline void default_color_syntax_command(gch::vector<str_subview> args, SyntaxInfo& out);
    inline void default_color_name_syntax_command(gch::vector<str_subview> args, SyntaxInfo& out);
    inline void default_opacity_syntax_command(gch::vector<str_subview> args, SyntaxInfo& out);

    inline void highlight_true(SyntaxInfo& out);

    const SyntaxMap geng_default_syntax_map {
        { // commands
            {"[s:@]", default_scale_syntax_command},
            {"[c:@]", default_color_name_syntax_command},
            {"[c:@,@,@]", default_color_syntax_command},
            {"[o:@]", default_opacity_syntax_command}
        },
        {}, // patterns
        {{"true", highlight_true}} // highlights
    };

    inline void default_scale_syntax_command(gch::vector<str_subview> args, SyntaxInfo& out) {
        gutils::cstr_to_float(args[0].cstr(), out.scale);
    }

    inline void default_color_syntax_command(gch::vector<str_subview> args, SyntaxInfo& out) {
        int r, g, b;
        gutils::cstr_to_int(args[0].cstr(), r);
        gutils::cstr_to_int(args[1].cstr(), g);
        gutils::cstr_to_int(args[2].cstr(), b);
        out.color.r = r;
        out.color.g = g;
        out.color.b = b;
    }

    inline void default_color_name_syntax_command(gch::vector<str_subview> args, SyntaxInfo &out) {
        if (args[0] == "red") out.color =  color_red;
        else if (args[0] == "green")  out.color = color_green;
        else if (args[0] == "blue")   out.color =  color_blue;
        else if (args[0] == "orange") out.color =  color_orange;
        else if (args[0] == "yellow") out.color =  color_yellow;
        else if (args[0] == "pink") out.color = color_pink;
        else if (args[0] == "white")  out.color = color_white;
        else if (args[0] == "black")  out.color =  {0, 0, 0, 255};
        else out.color = {255, 255, 255, 255};
    }

    inline void default_opacity_syntax_command(gch::vector<str_subview> args, SyntaxInfo &out) {
        int a;
        gutils::cstr_to_int(args[0].cstr(), a);
        out.color.a = a;
    }

    inline void highlight_true(SyntaxInfo &out) {
        out.color = {0,255,0,255};
    }
}
