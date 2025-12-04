#pragma once
#include <SDL_render.h>
#include <unordered_map>
#include <vector>

#include "FontChar.hpp"
#include "../utilities/BasisDecoder.hpp"

namespace gengine {
    class TextMap {
    private:
        // The location of each character on the spritesheet
        std::unordered_map<char, SDL_Rect> locs;
        SDL_Texture* tex = nullptr;
        std::string path;
    public:
        // consroctor
        TextMap(std::unordered_map<char, SDL_Rect> locs, std::string path)
            : locs(locs), path(path) {}
        // turns a string into a bunch of render rectangles
        std::vector<SDL_Rect*> string_to_font(std::string str) {
            std::vector<SDL_Rect*> result(str.length());
            for (int i = 0; i < str.length(); i++)
                result[i] = &locs[str[i]];
            return result;
        }
        // we need to make the texture :(
        void initialize_font(SDL_Renderer* rend) {
            tex = BasisDecoder::load_ktx2_as_texture(rend, path.c_str());
        }

    };


    static const std::unordered_map<char, SDL_Rect> default_font_rects = {
        {'a', {0, 0, 4, 9}}, // 'a'
        {'b', {5, 0, 4, 9}},
        {'c', {10, 0, 3, 9}},
        {'d', {14, 0, 4, 9}},
        {'e', {19, 0, 4, 9}}, // 'e'
        {'f', {24, 0, 3, 9}},
        {'g', {27, 0, 4, 9}},
        {'h', {32,0, 4, 9}},
        {'i',{37,0,1,9}},
        {'j',{38,0,2,9}},
        {'k',{41,0,4,9}},
        {'l', {46,0,1,9}},
        {'m',{48,0,7,9}},
        {'n',{56,0,4,9}},
        {'o', {61,0,4,9}},
        {'p',{66,0,4,9}},
        {'q',{71,0,4,9}},
        {'r',{76, 0, 3, 9}},
        {'s', {80, 0, 3, 9}},
        {'t',{84,0,4,9}},
        {'u',{88,0,4,9}},
        {'v',{93,0,5,9}},
        {'w',{99,0,7,9}},
        {'x',{107,0,5,9}},
        {'y',{113,0,4,9}},
        {'z', {118, 0, 4,9}},
        {'.', {50, 18, 1, 9}}
    };
}