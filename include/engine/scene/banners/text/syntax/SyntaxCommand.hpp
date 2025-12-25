#pragma once
#include <cstdint>
#include <SDL_pixels.h>

#include "SyntaxInfo.hpp"
#include "engine/debug/logging/LogSource.hpp"
#include "engine/types/strings/hstring/hstring.hpp"

namespace geng {

    /// blank function definitions
    inline void syntx_verif_blank(gch::vector<str_subview>, SyntaxInfo& out) { }
    inline void syntx_update_blank(SyntaxInfo& out) { }

    /** A SyntaxCommand is used to modify text via a specific parameter, and trigger a toggle.
     * The text within a syntax command will be ignored and change the rendering state.
     * @details For example, if I have a @code SyntaxCommand("[c:@]", SyntaxType::COLOR, my_func)@endcode, and then I have the text
     * @code "Hello world![c:blue]I'm blue now!@endcode, the str_subview "blue" will be bassed to the syntax command, and whatever color
     * is in the SyntaxCommand after execution of the my_func variable, the text's color will change to be that.
     */
    using SyntaxVerifier = void (*)(gch::vector<str_subview>, SyntaxInfo& out);
    class SyntaxCommand {
        /// The key to check the syntax command against
        hstring key;
        /// Function that verifies the data
        SyntaxVerifier verifier = syntx_verif_blank;
    public:

        SyntaxCommand(hstring key, SyntaxVerifier fn)
            : key(key), verifier(fn) {
            if (key.empty())
                glog::err.src("SyntaxCommand::SyntaxCommand") << "hstring key cannot be empty for SyntaxCommand." << glog::endlog;
            if (key[key.length()-1] == '@')
                glog::err.src("SyntaxCommand::SyntaxCommand") << "hstring key cannot end in '@'";
            if (key[0] == '@')
                glog::err.src("SyntaxCommand::SyntaxCommand") << "hstring key cannot start in '@'";
        }
        // Returns the number of characters to move forward to finish the command. 0 if the command fails to execute.
        int match(str_view str, int pos, SyntaxInfo& out) const {
            // Keeps track of internal index and string index
            int count = 0, i, wildcardpos = 0;
            // keeps track of whether we're in wildcard mode or not
            bool wildcard = false;
            // keeps track of the arguments the custom function will recieve.
            gch::vector<str_subview> arguments;
            for (i = pos; count < key.length() && i < str.length(); i++) {
                // If we're not in wildcard mode.
                if (!wildcard) {
                    if (key[count] == '@') {
                        wildcard = true;
                        wildcardpos = i;
                        count++;
                        continue;
                    }
                    if (key[count++] != str[i]) {
                        return 0;
                    }
                }
                // If we're in wildcard mode
                else {
                    // If we match the next character, end wildcard mode.
                    if (count < key.length() && key[count] == str[i]) {
                        wildcard = false;
                        arguments.push_back(str.subview(wildcardpos, i - wildcardpos));
                        count++;
                    }
                }
            }
            if (count == key.length()) {
                verifier(arguments, out);
                return i - pos;
            }
            return 0;
        }
    };

    /// The syntax pattern is used to identify common patterns of strings, such as numbers and the like.
    /// For example, the qualifier @code { if (str[pos] >= '0' && str[pos] <= '9') }@endcode will target all numbers.
    using SyntaxPattern = void (*)(str_view, int pos, SyntaxInfo& out);

    /// Syntax highlight highlights specific syntax structure easily.
    /// For example, if key == "true," it calls your SyntaxUpdate function easily.
    using SyntaxUpdate = void (*)(SyntaxInfo& out);
    struct SyntaxHighlight {
        hstring key;
        SyntaxUpdate update = syntx_update_blank;
        SyntaxHighlight(hstring key, SyntaxUpdate update) : key(key), update(update) {}

    };
}
