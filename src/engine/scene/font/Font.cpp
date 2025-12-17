#include "engine/scene/font/Font.hpp"

#include <iostream>


using namespace geng;


Font::Font(std::unordered_map<char, geng::Quad> quads, const short spacing, const short offset_x, const short offset_y) : spacing(spacing){
    for (auto& [c, q] : quads) {
        q.x += offset_x;
        q.y += offset_y;
        this->quads.emplace(c,q);
    }
    this->spacing = spacing;
}

Font::Font(const Font& f) : texture_id(f.texture_id), spacing(f.spacing), chars(f.chars), quads(f.quads) {}

short Font::get_height() const {
    if (chars.size() == 0 ) {
        std::cerr << "ERR: No Font Characters added.\n";
        return -1;
    }
    for (auto& [c, q] : chars)
        return q.h;
    return -1;
}

int Font::get_texture_id() const {
    return texture_id;
}

void Font::set_texture_id(int id) {
    std::cerr << "Font::Setting texture id??\n";
    texture_id = id;
}


void Font::add_FontChar(char c, std::vector<FontChar> &buffer, Vertex pos) {
    if (chars.find(c) != chars.end()) {
        FontChar f = chars.at(c);
        for (auto& i: f.points) {
            i.x += pos.x;
            i.y += pos.y;
        }
        buffer.push_back(chars.at(c));
    }
}

void Font::_init(IMG_Info info) {
    for (auto& [c, q] : quads) {
        chars.emplace(c, FontChar(q, info, 0));
    }
}
