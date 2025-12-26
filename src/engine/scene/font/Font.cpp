#include "engine/scene/font/Font.hpp"

#include "../../../../include/engine/debug/geng_debug.hpp"

using namespace geng;

Font::Font(std::unordered_map<char, geng::AnimBox2D> quads, const short spacing, const short offset_x, const short offset_y) : spacing(spacing){
    for (auto& [c, q] : quads) {
        q.x += offset_x;
        q.y += offset_y;
        this->quads.emplace(c,q);
    }
}

Font::Font(std::unordered_map<char, AnimBox2D> quads,
    short spacing, IMG_Info info) : spacing(spacing), quads(quads) {
    texture_id = 0;
    _init(info);
    glog::note.src("Font") << "special constructor" << glog::endlog;
}

Font::Font(const Font& f) : texture_id(f.texture_id), spacing(f.spacing), chars(f.chars), quads(f.quads) {}

short Font::get_height() const {
    if (chars.size() == 0 ) {
        //std::cerr << "ERR: No Font Characters added.\n";
        return -1;
    }
    for (auto& [c, q] : chars)
        return q.h;
    return -1;
}

int Font::get_texture_id() const {
    return texture_id;
}

float Font::get_spacing() const {
    return spacing;
}

bool Font::has_character(char c) {
    return chars.find(c) != chars.end();
}

void Font::set_texture_id(int id) {
    texture_id = id;
}

FontChar * Font::get_char(char c) {
    if (chars.find(c) != chars.end())
        return &chars.at(c);
    return nullptr;
}

uint16_t Font::get_width(char c) {
    if (chars.find(c) != chars.end())
        return chars.at(c).w;
    return 0;
}

uint16_t Font::get_height(char c) {
    if (chars.find(c) != chars.end())
        return chars.at(c).h;
    return 0;
}

str_view & Font::to_fstring(str_view& buffer) {
    buffer << "Font: texid: " << texture_id << " spacing: " << spacing << "\n";
    buffer << "characters: ";
    for (auto& [c, fc] : chars) {
        buffer << c << ':';
        fc.to_fstring(buffer) << '\n';
    }
    return buffer;
}


bool Font::add_char_to_buffer(char c, gch::vector<FontChar> &buffer) {
    if (chars.find(c) != chars.end()) {
        FontChar f = chars.at(c);
        buffer.push_back(chars.at(c));
        return true;
    }
    return false;
}

void Font::_init(IMG_Info info) {
    for (auto& [c, q] : quads) {
        chars.emplace(c, FontChar(q, info, 0));
    }
}
