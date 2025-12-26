#pragma once
#include <SDL_render.h>

#include "engine/core/defaults/DefaultSyntaxMap.hpp"
#include "engine/scene/font/Font.hpp"
#include "engine/scene/banners/widgets/Widget.hpp"
#include "syntax/SyntaxMap.hpp"

namespace geng {
    namespace debug {
        class Console;
    }
    class Renderer;

    /** Simple debug text rendering **/
    template<uint16_t C>
    class Text : public Widget {
    protected:
        /// The actual text stripped of commands
        fstring<C> textliteral;
        /// The font this text uses
        Font& font;
        /// Cached vertices for later use
        gch::vector<SDL_Vertex> vertices;
        /// A pointer to the SyntaxMap this text uses
        const SyntaxMap* syntax_map = &geng_default_syntax_map;
        friend Renderer;
        friend debug::Console;
    public:
        Text(const char text[], Font& font)
            : font(font),
              Widget(0, 0, 0, 0, font.get_texture_id())
        {
            Text<C>::update_text(text);
            texture_id = font.get_texture_id();
        }

        Text(const char text[], Font& font, const SyntaxMap* syntax_map)
            : Widget(0,0,0,0, font.get_texture_id()), font(font), syntax_map(syntax_map)
        {
            Text<C>::update_text(text);
            texture_id = font.get_texture_id();
        }

        void change_dim(Dim2D dimensions) override {
            box.w = dimensions.w;
            box.h = dimensions.h;
        }

        int to_vertex(BannerBuffer& buffer) override {
            // Copy to buffer
            for (auto& v : vertices) buffer.push_back(v);
            return static_cast<int>(vertices.size());
        }

        [[nodiscard]] fstring<C> get_text() const {
            return textliteral;
        }

        [[nodiscard]] geng::str_view get_fstr_view() {
            return textliteral.wrap();
        }

        void clear() {
            vertices.clear();
            textliteral.clear();
        }

        virtual void update_text(const char text[]) {
            // Clears out all the cached data
            clear();
            // make our font character buffer
            gch::vector<FontChar> buffer;
            // create a wrapper text
            fstring<C + 400> new_text(text);
            // Grab any syntax information
            SyntaxInfo syntax_info[C];
            // Have our syntax map feed information into our buffer & syntax info array.
            textliteral = syntax_map->parse<C>(new_text.wrap(), font, buffer, syntax_info);
            // Then we render each of our font characters accordingly.
            if (buffer.size() == 0) return;
            // current position
            Pos2D pos = {box.x, box.y};
            // maximum height of the current line
            uint16_t max_height = buffer[0].h*syntax_info[0].scale;
            // Increment through our buffer
            for (int i = 0, k = 0; i < buffer.size(); i++, k++) {
                // If we have whitespace, that's characters that textliteral has that syntax_info does not.
                if (textliteral[k] == '\n') {
                    pos.y += max_height;
                    max_height = 0;
                    pos.x = box.x;
                    i--;
                    continue;
                }
                if (textliteral[k] == '\t') {
                    pos.x += 24 - (pos.x % 24);
                    i--;
                    continue;
                }
                if (textliteral[k] == ' ') {
                    pos.x += 4*font.get_spacing();
                    i--;
                    continue;
                }
                buffer[i].to_vertex(vertices, pos, syntax_info[i].color, syntax_info[i].scale);
                pos.x += (buffer[i].w + font.get_spacing())*syntax_info[i].scale;
                if (buffer[i].h * syntax_info[i].scale > max_height)
                    max_height = buffer[i].h * syntax_info[i].scale;
            }
        }

        [[nodiscard]] Font* get_font(){
            return &font;
        }
    };
}