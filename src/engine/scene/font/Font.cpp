#include "engine/scene/font/Font.hpp"

#include "engine/debug/logging/LogSource.hpp"

using namespace geng;

Font::Font(std::unordered_map<char, geng::AnimBox2D> quads, const short spacing, const short offset_x, const short offset_y) : spacing(spacing){
    for (auto& [c, q] : quads) {
        q.x += offset_x;
        q.y += offset_y;
        this->quads.emplace(c,q);
    }
}

Font::Font(std::unordered_map<char, AnimBox2D> quads,
    short spacing, IMG_Info info) {
    this->quads = quads;
    _init({"sys-font.png",128, 128} );
    glog::note.src("Font") << "special constructor";
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
    glog::note << "Font::Setting texture id??\n";
    texture_id = id;
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
