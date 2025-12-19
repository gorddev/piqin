#include "engine/scene/banners/text/TextParser.hpp"

#include <iostream>

using namespace geng;

static SDL_Color default_color() {
    return {255, 255, 255, 255};
}

SDL_Color TextParser::color_from_name(const std::string& name) {
    if (name == "red")    return {255, 0, 0, 255};
    if (name == "green")  return {0, 255, 0, 255};
    if (name == "blue")   return {0, 0, 255, 255};
    if (name == "orange") return {255, 165, 0, 255};
    if (name == "white")  return {255, 255, 255, 255};
    if (name == "black")  return {0, 0, 0, 255};

    return default_color(); // default white
}

std::string TextParser::trim(const std::string& s) {
    size_t a = s.find_first_not_of(" \t\n");
    size_t b = s.find_last_not_of(" \t\n");
    return (a == std::string::npos) ? "" : s.substr(a, b - a + 1);
}

/// Returns what you will need to edit from the parser.
ParseEvent TextParser::parse_next(const std::string& src, int pos) {
    ParseEvent out;

    if (pos >= (int)src.size()) {
        out.type = ParseType::End;
        out.advance = 0;
        return out;
    }

    // text
    if (src[pos] != '^') {
        int start = pos;
        while (pos < static_cast<int>(src.size()) && src[pos] != '^')
            pos++;

        out.type = ParseType::RawText;
        out.text = src.substr(start, pos - start);
        out.advance = pos - start;
        return out;
    }

    // command
    int cmd_end = src.find('^', pos + 1);

    // malformed '^' → literal
    if (cmd_end == std::string::npos) {
        out.type = ParseType::RawText;
        out.text = "^";
        out.advance = 1;
        return out;
    }

    std::string cmd = src.substr(pos + 1, cmd_end - pos - 1);
    int consumed = cmd_end - pos + 1; // include both ^

    // scale
    if (cmd.rfind("s:", 0) == 0) {
        std::string val = trim(cmd.substr(2));
        if (!val.empty() && val.back() == 'f')
            val.pop_back();

        out.type = ParseType::Scale;
        out.scale = std::stof(val);
        out.advance = consumed;
        return out;
    }

    // color
    if (cmd.rfind("c:", 0) == 0) {
        std::string val = trim(cmd.substr(2));
        out.type = ParseType::Color;

        if (!val.empty() && std::isalpha(val[0])) {
            out.color = color_from_name(val);
            out.advance = consumed;
            return out;
        }

        int nums[4] = {255, 255, 255, 255};
        int count = 0;
        size_t p = 0;

        while (p < val.size() && count < 4) {
            size_t comma = val.find(',', p);
            std::string tok = trim(val.substr(p, comma - p));
            nums[count++] = std::stoi(tok);
            if (comma == std::string::npos) break;
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
}

