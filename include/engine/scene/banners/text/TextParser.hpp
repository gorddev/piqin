#pragma once
#include <SDL_pixels.h>
#include "../../../types/strings/fstring/fstring.hpp"
#include "engine/types/external/vector.hpp"


namespace gan {

    /// Parse list is just a vector containing all the things you want to parse for
    using ParseList = gch::vector<int>;

    enum ParseType { RawText, Scale, Color, End };

    /// Contains one type event to extract
    struct ParseEvent {
        ParseType type;
        fstring<20> text;
        float scale = 1.0f;
        SDL_Color color{};
        int advance = 0;
    };

    /**
     * TextParser parses text and looks for user codes to render text in specific ways.
     * Current avaliable scancodes
     */
    class TextParser {
    private:
        static SDL_Color color_from_name(const str_view& name);

    public:
        /// Returns what you will need to edit from the parser.
        static ParseEvent parse_next(const str_view& src, int pos);
    };

}
