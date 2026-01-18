#pragma once
#include <SDL_render.h>

#include "engine/core/defaults/DefaultSyntaxMap.hpp"
#include "engine/scene/font/Font.hpp"
#include "engine/scene/banners/widgets/Widget.hpp"
#include "syntax/SyntaxMap.hpp"

namespace gan {
    class Renderer;

    /** Simple debug text rendering **/
    template<uint16_t C>
    class Text : public Widget {
    protected:
        /// The actual text stripped of commands
        fstring<C> textliteral;
        /// The font this text uses
        Font* font;
        /// Cached vertices for later use
        gch::vector<SDL_Vertex> vertices;
        /// Keeps track of the total height of the text
        float height = 0;
        /// A pointer to the SyntaxMap this text uses
        const SyntaxMap* syntax_map = &gan_default_syntax_map;
        friend Renderer;
    public:
        Text(const char text[], Font& font)
            : font(&font),
              Widget(0, 0, 0, 0, font.get_texture_id())
        {
            Text<C>::update_text(text);
            texture_id = font.get_texture_id();
        }

        Text(const char text[], Font& font, const SyntaxMap* syntax_map)
            : Widget(0,0,0,0, font.get_texture_id()), font(&font), syntax_map(syntax_map)
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

        [[nodiscard]] gan::str_view get_fstr_view() {
            return textliteral.wrap();
        }

        void clear() {
            vertices.clear();
            textliteral.clear();
        }

        virtual void update_text(const char text[]) {
            clear(); // Clear previous buffer

            gch::vector<FontChar> buffer;
            fstring<C + 400> new_text(text);

            SyntaxInfo syntax_info[C];
            textliteral.clear();
            textliteral = syntax_map->parse<C>(new_text.wrap(), *font, buffer, syntax_info);

            if (buffer.empty()) return;

            Pos2D pos { box.x, box.y };
            uint16_t lineHeight = font->get_metrics().lineSkip;

            for (int i = 0; i < textliteral.length(); ++i) {
                char c = textliteral[i];
                if (c == '\n') {
                    pos.y += lineHeight;
                    pos.x = box.x;
                    continue;
                } if (c == '\t') {
                    pos.x += 32 - (pos.x%32);
                    continue;
                }
                buffer[i].to_vertex(vertices, pos, syntax_info[i].color, syntax_info[i].scale);
                pos.x += buffer[i].advance * syntax_info[i].scale;
                lineHeight = std::max(lineHeight, static_cast<uint16_t>(buffer[i].dim.h * syntax_info[i].scale));
            }
            height = pos.y + lineHeight;
        }

        [[nodiscard]] Font* get_font(){
            return font;
        }

        [[nodiscard]] float get_height() {
            return height;
        }
    };
}