#include "engine/scene/banners/text/Text.hpp"

#include <iostream>

using namespace geng;

void Text::update_character_reserve(const std::string& string_text) {
    characters.reserve(string_text.length());
    bool command = false;
    for (int i = 0; i < string_text.length(); i++) {
        /// If it's a newline we move to a new line and record it in our textliteral
        if (string_text[i] == '^') {
            command = !command;
            textliteral += '^';
            continue;
        }
        if (command || string_text[i] == '\n' || string_text[i] == ' ' || string_text[i] == ':') {
            textliteral += string_text[i];
            continue;
        }
        if (font.add_char_to_buffer(string_text[i], characters)) {
            textliteral += string_text[i];
        }
    }
}

geng::Text::Text(const std::string& string_text, Font& font)
    : font(font),
      Widget(0, 0, 0, 0, font.get_texture_id()) {
    update_character_reserve(string_text);
    texture_id = font.get_texture_id();
    std::cerr << "setting text texture id: " << texture_id << std::endl;
}

/** Changes nothing about the text (for now) **/
void Text::change_dim(Dim2D dimensions) {
    dim.w = dimensions.w;
    dim.h = dimensions.h;
}

/** Changes the text of a text object **/
int Text::to_vertex(BannerBuffer& buffer) {
    if (vertices.empty()) {
        /// Current running position of the transformer
        Pos2D pos = {0 ,0};
        /// Current running color of the transformer
        SDL_Color color = {255, 255, 255, 255};
        /// Current scale of the text
        float scale = 1.f;

        int count = 0;
        int vertexCount = 0;
        // Iterate through our string
        std::cerr << "textliteral: " << textliteral << std::endl;
        for (int i = 0; i < textliteral.length(); i++) {
            // If we encounter a command code
            if (textliteral[i] == '^') {
                ParseEvent e = TextParser::parse_next(textliteral, i);
                if (e.type == Color) {
                    color = e.color;
                }
                else if (e.type == Scale)
                    scale = e.scale;
                // Make sure to advance & continue
                i += e.advance;
                continue;
            }
            // Otherwise if we encounter a newline
            if (textliteral[i] == '\n') {
                pos.x = 0;
                pos.y += font.get_height();
                continue;
            }
            if (textliteral[i] == ' ') {
                pos.x += characters[0].w;
                continue;
            }
            // If we pass those tests we can actually render the font.
            characters[count].to_vertex(vertices, pos, color, scale);
            // Advance x
            pos.x += characters[count].w * scale + font.get_spacing();
            // Add to the FPos2D count
            vertexCount += 6;
            // Add to count:
            count++;
        }
    }
    for (auto& v : vertices)
        buffer.push_back(v);
    return vertices.size();
}

/// to_string function
std::string Text::to_string() const {
    std::string ret = "geng::Text: size: " + std::to_string(characters.size()) + "\n";
    for (auto& i : characters) {
        ret += i.to_string();
    }
    return ret;
}

