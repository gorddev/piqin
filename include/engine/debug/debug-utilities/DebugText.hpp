#pragma once
#include "default-debug-syntax-map.hpp"
#include "engine/scene/banners/text/Text.hpp"

namespace gan::debug {

    /** Text used for debugging, which has a black background */
    template<uint16_t C>
    class DebugText : public Text<C> {
    private:

    public:
        DebugText(const char text[], Font& font)
            : Text<C>(text, font, &gan_default_debug_syntax_map) {}


        int to_vertex(BannerBuffer& buffer) {
            // 'vertices' is your SDL_Vertex vector storing all character quads

            Text<C>::to_vertex(buffer);
            return 0;
            gch::vector<SDL_Vertex>& vv = this->vertices;
            fstring<C>& text = this->textliteral;
            int size = static_cast<int>(text.length());

            SDL_FPoint sp = vv.empty() ? SDL_FPoint{0,0} : vv[0].position;
            SDL_FPoint ep = vv.empty() ? SDL_FPoint{0,0} : vv[1].position;

            int i = 0; // index into vertices

            for (int k = 0; k < size; ++k) {
                if (text[k] == ' ' || text[k] == '\t') {
                    continue;
                }

                // Each glyph quad consists of 6 vertices
                int quadStart = i;
                int quadEnd = i + 6;

                if (quadEnd > static_cast<int>(vv.size())) {
                    // Prevent overflow if vertex count is off
                    break;
                }

                // Handle newline characters
                if (text[k] == '\n') {
                    // Search backward for last non-newline char to draw box
                    auto backsearch = [&](){
                        for (int l = k - 1; l >= 0; --l) {
                            if (text[l] == '\n')
                                return -1; // no valid char found
                            if (text[l] != '\t' && text[l] != ' ')
                                return l;
                        }
                        return -1;
                    };

                    int lastCharIndex = backsearch();
                    if (lastCharIndex != -1 && quadStart - 6 >= 0) {
                        // Draw bounding box around previous line
                        ep = vv[quadStart - 1].position;
                        --sp.x;
                        ++ep.x;

                        buffer.push_back(sp, debug_grey);
                        buffer.push_back({sp.x, ep.y}, debug_grey);
                        buffer.push_back({ep.x, ep.y}, debug_grey);

                        buffer.push_back(sp, debug_grey);
                        buffer.push_back(ep, debug_grey);
                        buffer.push_back({ep.x, sp.y}, debug_grey);
                    }

                    // Reset start point for new line
                    sp = vv[quadStart].position;
                } else {
                    // For normal glyphs, push their vertices into the buffer
                    for (int vi = quadStart; vi < quadEnd; ++vi) {
                        buffer.push_back(vv[vi]);
                    }
                    i += 6;
                }
            }

            // Return number of vertices added to buffer
            return text.length() * 2;
        }

    };

}
