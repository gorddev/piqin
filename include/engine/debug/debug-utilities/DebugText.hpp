#pragma once
#include "default-debug-syntax-map.hpp"
#include "engine/scene/banners/text/Text.hpp"

namespace geng::debug {

    /** Text used for debugging, which has a black background */
    template<uint16_t C>
    class DebugText : public Text<C> {
    private:

    public:
        DebugText(const char text[], Font& font)
            : Text<C>(text, font, &geng_default_debug_syntax_map) {}

        int to_vertex(BannerBuffer& buffer) {
            // get the vector of vertices
            gch::vector<SDL_Vertex>& vv = (this->vertices);
            // get the actual text
            fstring<C>& text = this->textliteral;
            // get the size of the text;
            int size = text.length();
            SDL_FPoint sp = {vv[0].position};
            SDL_FPoint ep = {vv[1].position};
            int i = 0;
            for (int k = 0; k < size; i+=6, k++) {
                if (text[k] == ' ' || text[k] == '\t') {
                    i-=6;
                    continue;
                }
                // search to find the last character that isn't '\n'
                auto backsearch = [&]() {
                    // search for the last non-newline character.
                    for (int l = k - 1; l > 0; l--) {
                        if (text[l] == '\n')
                            return 0;
                        if (text[l] != '\t' && text[l] != ' ')
                            return l;
                    }
                    return 0;
                };
                if (text[k] == '\n') {
                    int search = backsearch();
                    if (search != 0) {
                        ep = vv[i-1].position;
                        --sp.x;
                        ++ep.x;
                        buffer.push_back(sp, debug_grey);
                        buffer.push_back({sp.x, ep.y}, debug_grey);
                        buffer.push_back({ep.x, ep.y}, debug_grey);
                        buffer.push_back(sp, debug_grey);
                        buffer.push_back(ep, debug_grey);
                        buffer.push_back({ep.x, sp.y}, debug_grey);

                    }
                    sp = vv[i].position;
                    i-=6;
                }
            }
            for (auto& v : vv)
                buffer.push_back(v);
            return static_cast<int>(2*vv.size());
        }
    };

}
