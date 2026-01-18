#include "engine/scene/font/Font.hpp"

#include "engine/debug/geng_debug.hpp"
#include "engine/types/positioning/FPos2D.hpp"

using namespace gan;

Font::Font(hstring path, uint16_t pt, uint16_t padding)
    : font(TTF_OpenFont(path.c_str(), pt)), padding(padding), pt(pt){
    if (!font) {
        glog::err << "could not load font: " << TTF_GetError() << glog::endlog;
        throw std::runtime_error(TTF_GetError());
    }
    chars.resize(1+'~'-' ');
    metrics.lineSkip = TTF_FontLineSkip(font);
    metrics.ascent = TTF_FontAscent(font);
    metrics.descent = TTF_FontDescent(font);
}

// constructor used for making the system font.
Font::Font(int gw, int gh, int tw, int th, int pad, int texid)
    : texture_id(texid), font(nullptr)
{
    // make sure to allocate enough space
    chars.resize(1+'~' - ' ');
    // width of a glyph + padding
    int cw = gw + pad * 2;
    // height of a glyph + padding
    int ch = gh + pad * 2;
    // how much we increase in x each time
    int stepX = cw - 1;
    // same thing for y
    int stepY = ch - 1;

    // the character we start on.
    int charnum = ' ';
    for (int h = 0; h + ch <= th; h += stepY) {
        for (int w = 0; w + cw <= tw; w += stepX) {
            if (charnum > '~') break;
            float u0 = (w + 0.5f) / tw;
            float v0 = (h + 0.5f) / th;
            float u1 = (w + cw - 0.5f) / tw;
            float v1 = (h + ch - 0.5f) / th;

            gch::vector<SDL_FPoint> uv(4);
            uv[0] = { u0, v0 }; // TL
            uv[1] = { u1, v0 }; // TR
            uv[2] = { u0, v1 }; // BL
            uv[3] = { u1, v1 }; // BR
            int advance = gw + 1;
            if (charnum == '[')
                advance = gw - 3;
            else if (charnum == ' ')
                advance = gw - 2;
            else if (charnum == 'm' || charnum == 'w')
                advance = gw +1;
            else if (charnum >= 'a')
                advance = gw;
            FontChar mychar(uv, {cw, ch}, advance);
            mychar.character = charnum;
            chars[charnum++ - 32] = (mychar);
        }
    }
    metrics.glyphW  = gw;
    metrics.glyphH  = gh;
    metrics.advance = gw + pad * 2 - 1;
    metrics.ascent  = gh;
    metrics.descent = 0;
    metrics.lineSkip = gh + pad * 2;
}


void Font::add_char(char c, Dim2D atlas_dim, Pos2D tex_pos, Dim2D tex_dim, uint16_t advance) {
    if (has_character(c)) {
        gch::vector<SDL_FPoint> vertexPoints(4);
        float u0 = (tex_pos.x - 0.5f) / atlas_dim.w;
        float v0 = (tex_pos.y - 0.5f) / atlas_dim.h;
        float u1 = (tex_pos.x + tex_dim.w + 0.5f) / atlas_dim.w;
        float v1 = (tex_pos.y + tex_dim.h + 0.5f) / atlas_dim.h;
        vertexPoints[0] = { u0, v0 }; // top-left
        vertexPoints[1] = { u1, v0 }; // top-right
        vertexPoints[2] = { u0, v1 }; // bottom-left
        vertexPoints[3] = { u1, v1 };
        chars[c - 32] = FontChar(vertexPoints, tex_dim, advance);
    }
    else
        glog::warn << "adding character " << c << " that is not ascii." << glog::endlog;
}

int Font::get_texture_id() const {
    return texture_id;
}

int Font::get_padding() const {
    return padding;
}

int Font::get_pt() const {
    return pt;
}

bool Font::has_character(char c) {
    return (c >= 32 && c <= 127);
}

void Font::set_texture_id(int id) {
    texture_id = id;
}

FontChar* Font::get_char(char c) {
    if (has_character(c))
        return &chars[c - 32];
    return nullptr;
}

uint16_t Font::get_width(char c) {
    if (has_character(c))
        return chars[c- 32].dim.w;
    return 0;
}

uint16_t Font::get_height(char c) {
    if (has_character(c))
        return chars[c - 32].dim.h;
    return 0;
}

FontMetrics Font::get_metrics() {
    return metrics;
}

bool Font::add_char_to_buffer(char c, gch::vector<FontChar> &buffer) {
    glog::dev << "font ttf: " << font << glog::endlog;
    glog::dev << "adding character " << int(c) << " to buffer" << glog::endlog;
    if (has_character(c)) {
        buffer.push_back(chars[c - 32]);
        return true;
    }
    return false;
}

str_view & Font::to_fstring(str_view& buffer) {
    /*
    buffer << "Font: texid: " << texture_id << " padding: " << padding << "\n";
    buffer << "characters: ";
    for (char i = 0; i < chars.size(); i++) {
        buffer << (i - 32) << ": ";
        chars[i].to_fstring(buffer) << '\n';
    }
    */
    return buffer;
}

TTF_Font * Font::get_font() const {
    return font;
}

