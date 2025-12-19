#pragma once
#include <string>

namespace geng {
    /**
     * Used for returning information to the user about the image
     */
    struct IMG_Info {
        std::string filename = "null";
        uint32_t w;
        uint32_t h;
        IMG_Info(std::string fn, uint32_t w, uint32_t h)
            : filename(std::move(fn)), w(w), h(h) {}

        std::string to_string() const {
            std::string ret = "Filename: " + filename + " w: " + std::to_string(w) + " h: " + std::to_string(h);
            return ret;
        }
    };
}
