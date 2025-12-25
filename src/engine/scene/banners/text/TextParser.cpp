#include "engine/scene/banners/text/TextParser.hpp"

#include "../../../../../include/engine/types/strings/fstring/fstring.hpp"

using namespace geng;


SDL_Color TextParser::color_from_name(const geng::str_view& name) {
    if (name == "red")    return {255, 60, 60, 255};
    if (name == "green")  return {100, 200, 100, 255};
    if (name == "blue")   return {140, 140, 255, 255};
    if (name == "orange") return {255, 165, 0, 255};
    if (name == "white")  return {255, 255, 255, 255};
    if (name == "black")  return {0, 0, 0, 255};
    return {255, 255, 255, 255};
}

/// Returns what you will need to edit from the parser.
ParseEvent TextParser::parse_next(const geng::str_view& src, int pos) {

    ParseEvent out;
    out.type = ParseType::End;
    out.advance = 0;
    /*

    for (int i = pos; i < src.length(); i++) {

    }

    if (pos >= static_cast<int>(src.length())) {
        out.type = ParseType::End;
        out.advance = 0;
        return out;
    }

    // text
    if (src[pos] != '^') {
        int start = pos;
        while (pos < static_cast<int>(src.length()) && src[pos] != '^')
            pos++;

        out.type = ParseType::RawText;
        //out.text = fstring<256>(src.cstr() + start, pos - start);
        out.advance = pos - start;
        return out;
    }

    // command
    int cmd_end = -1;
    for (int i = pos + 1; i < static_cast<int>(src.length()); i++) {
        if (src[i] == '^') {
            cmd_end = i;
            break;
        }
    }

    // malformed '^' → literal
    if (cmd_end == -1) {
        out.type = ParseType::RawText;
        out.text = "^";
        out.advance = 1;
        return out;
    }

    geng::str_view cmd = src.wrap().str_subview(pos + 1);
    cmd = cmd.substr<128>(0); // up to cmd_end - pos - 1
    int consumed = cmd_end - pos + 1; // include both ^

    // scale
    if (cmd.substr<2>(0) == "s:") {
        geng::fstring<128> val = trim(cmd.substr<128>(2));
        if (!val.empty() && val[val.length() - 1] == 'f')
            val[len - 1] = '\0';

        out.type = ParseType::Scale;
        out.scale = std::stof(val.cstr());
        out.advance = consumed;
        return out;
    }

    // color
    if (cmd.substr<2>(0) == "c:") {
        fstring<128> val = trim(cmd.substr<128>(2));
        out.type = ParseType::Color;

        if (!val.empty() && std::isalpha(val[0])) {
            out.color = color_from_name(val.wrap());
            out.advance = consumed;
            return out;
        }

        int nums[4] = {255, 255, 255, 255};
        int count = 0;
        uint16_t p = 0;

        while (p < val.length() && count < 4) {
            uint16_t comma = p;
            while (comma < val.length() && val[comma] != ',') comma++;
            fstring<32> tok(val.cstr() + p, comma - p);
            nums[count++] = std::stoi(tok.cstr());
            if (comma == val.length()) break;
            p = comma + 1;
        }

        out.color = {
            (uint8_t)nums[0],
            (uint8_t)nums[1],
            (uint8_t)nums[2],
            (uint8_t)nums[3]
        };
        out.advance = consumed;
        return out;
    }

    // Skip it silently
    out.type = ParseType::RawText;
    out.text = "";
    out.advance = consumed;
    return out;
    */
    return out;
}