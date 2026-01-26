#pragma once
#include <SDL_render.h>

#include "engine/core/defaults/syntax-maps/DefaultSyntaxMap.hpp"
#include "engine/scene/font/Font.hpp"
#include "engine/mods/plugins/banners/widgets/Widget.hpp"
#include "syntax/SyntaxMap.hpp"

namespace gan {
    class Renderer;

    /** Simple debug text rendering **/
    class Text : public Widget {
    protected:
        /// The actual text stripped of commands
        hstring textliteral;
        /// The font this text uses
        Font* font;
        /// Cached vertices for later use
        std::vector<SDL_Vertex> vertices;
        /// Keeps track of the total height of the text
        float height = 0;
        /// Keeps track of the alignment of the text
        Align alignment;
        /// A pointer to the SyntaxMap this text uses
        const SyntaxMap* syntax_map;
        friend Renderer;
    public:

        Text(const char text[], Font& font, Align alignment = Align::LEFT, const SyntaxMap* syntax_map = &default_syntax_map)
            : Widget(0,0,0,0, font.get_texture_id()), font(&font), syntax_map(syntax_map), alignment(alignment)
        {
            Text::update_text(text);
            texture_id = font.get_texture_id();
        }

        void change_dim(dim2 dimensions) override {
            box.w = dimensions.w;
            box.h = dimensions.h;
        }

        int to_vertex(BannerBuffer& buffer) override {
            // Copy to buffer
            for (auto& v : vertices) buffer.push_back(v);
            return static_cast<int>(vertices.size());
        }

        [[nodiscard]] hstring get_text() {
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

            // create a buffer to store each of our characters
            std::vector<FontChar> buffer;
            // create a blank fstring to store all of our text.
            hstring new_text(text);

            // Holds all the info to syntax highlight each block of text
            std::vector<SyntaxInfo> syntax_info;
            syntax_info.resize(new_text.length());
            // Clears our current textliteral string
            textliteral.clear();
            // Updates our text literal with the command-removed parsed string, as well as adds characters to our fontchar buffer
            textliteral = syntax_map->parse(new_text.wrap(), *font, buffer, syntax_info).c_str();
            // if we have nothing in our buffer
            if (buffer.empty()) return;

            // position of the text as we write to it.
            pos2 pos { box.x, box.y };
            // keeps track of the current height of our lines.
            uint16_t lineHeight = font->get_metrics().lineSkip;

            // Start looping through our textliteral and syntaxinfo objects.
            for (int i = 0; i < textliteral.length(); ++i) {
                // Grab the character from our current textliteral
                char& c = textliteral[i];
                // If we encounter a newline
                if (c == '\n') {
                    // Add to our current running position.
                    pos.y += lineHeight;
                    pos.x = box.x;
                    continue;
                } if (c == '\t') {
                    pos.x += 32*syntax_info[i].scale - (pos.x%static_cast<int>(32*syntax_info[i].scale));
                    continue;
                }
                buffer[i].to_vertex(vertices, pos, syntax_info[i].color, syntax_info[i].scale);
                pos.x += (buffer[i].advance + font->get_padding()) * syntax_info[i].scale;
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

        void align(dim2 banner_dim) override {
            // establish counters to keep track of vertices
            uint32_t total_count = 0, new_count = 0;

            // Iterate our string and search for newlines

            for (int i = 0; i < textliteral.length(); ++i) {
                int j;
                // STEP 1: Search for newlines and add to new counter
                for (j = i; j < textliteral.length() && textliteral[j] != '\n'; ++j) {
                    if (font->has_character(textliteral[j])) {
                        new_count++;
                    }
                }

                // now we establish a total width for a current number of vertices
                double line_width = vertices[new_count*6 - 1].position.x;
                // then we calculate a new offset to move our line by
                pos2 offset;
                if (alignment == Align::CENTER)
                    offset.x = banner_dim.w/2 - line_width/2;
                if (alignment == Align::RIGHT)
                    offset.x = banner_dim.w - line_width;


                // now we iterate through each of the vertices and update with the offset.

                for (int c = total_count; c < new_count ; c++) {
                    for (int v = 0; v < 6; v++) {
                        vertices[c*6 + v].position.x += offset.x;
                        vertices[c*6 + v].position.y += offset.y;
                    }
                }

                i = j;
                total_count = new_count;
            }
        }

        void resize_internal_string(uint16_t new_length) {
            textliteral.resize(new_length);
        }

        hstring& get_internal_string() {
            return textliteral;
        }
    };
}