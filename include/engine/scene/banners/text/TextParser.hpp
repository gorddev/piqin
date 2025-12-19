#pragma once
#include <SDL_pixels.h>
#include <string>

namespace geng {

    enum ParseType { RawText, Scale, Color, End };

    /// Contains one type event to extract
    struct ParseEvent {
        ParseType type;
        std::string text;
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
        static SDL_Color color_from_name(const std::string& name);
        static std::string trim(const std::string& s);

    public:
        /// Returns what you will need to edit from the parser.
        static ParseEvent parse_next(const std::string& src, int pos);
    };

}