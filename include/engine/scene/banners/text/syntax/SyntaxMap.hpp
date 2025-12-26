#pragma once

#include <functional>

#include "SyntaxCommand.hpp"
#include "engine/scene/font/Font.hpp"
#include "engine/scene/font/FontChar.hpp"
#include "engine/types/external/vector.hpp"

namespace geng {

    /** The SyntaxMap can be used to modify the display of specific parts of text via SyntaxCommands.
     * Specifying a SyntaxMap with a piece of text will have the text automatically modify the displayed text based on your parameters.*/
    class SyntaxMap {
    private:
        /// Contains al the syntax commands the user will uase
        gch::vector<SyntaxCommand> commands;
        /// Contains all the SyntaxPattersn the user will use
        gch::vector<SyntaxPattern> patterns;
        /// Contains all the highlights the user will use.
        gch::vector<SyntaxHighlight> highlights;
    public:
        /// Constructor for a syntax map
        SyntaxMap(gch::vector<SyntaxCommand> commands,
            gch::vector<SyntaxPattern> patterns,
            gch::vector<SyntaxHighlight> highlights)
                : commands(commands), patterns(patterns),
                highlights(highlights) {}

        /// Parses through a block of text (oh goody!) and appends appropriate font characters.
        /// Returns an fstring of appropriate size
        template<uint32_t C>
        fstring<C> parse(str_view text, Font& font, gch::vector<FontChar>& buffer, SyntaxInfo* syntax_info) const {
            // Clear out the fontchar buffer
            buffer.clear();
            // Create our return fstring
            fstring<C> ret;
            // Creates a temporary syntax info object
            SyntaxInfo info;
            // Allows us to keep track of
            SyntaxInfo tempinfo = info;
            // Length of the text
            long len = std::min(text.length(), C);
            // Then we start iterating through our text
            uint32_t g = 0;
            for (int i = 0; i < len; i++) {
                if (text[i] == '\t' || text[i] == ' ' || text[i] == '\n') {
                    ret << text[i];
                    continue;
                }
                // Apply each of our commands
                bool command = false;
                for (auto& c : commands) {
                    int advance = c.match(text, i, info);
                    if (advance > 0) {
                        i += advance - 1;
                        command = true;
                        break;
                    }
                }
                if (command) continue;
                // Create a temporary info object for instanced patterns & highlights
                tempinfo = info;
                // Go through each of our patterns
                for (auto& p : patterns)
                    p(text, i, tempinfo);
                // Go through each of our highlights
                bool highlighted = false;
                for (auto& h : highlights) {
                    // If a highlight matches
                    if (len - i >= h.key.length() && text.subview(i, h.key.length()) == h.key.cstr()) {
                        // Update our temporary info
                        h.update(tempinfo);
                        for (int k = 0; k < h.key.length(); k++) {
                            if (font.add_char_to_buffer(text[i+k], buffer)) {
                                syntax_info[g +k] = tempinfo;
                                ret << text[i+k];
                            }
                        }
                        highlighted = true;
                        g += h.key.length();
                        i += h.key.length() - 1;
                    }
                }
                if (!highlighted && font.add_char_to_buffer(text[i], buffer)) {
                    syntax_info[g++] = tempinfo;
                    ret << text[i];
                }
            }
            return ret;
        }
    };
}
