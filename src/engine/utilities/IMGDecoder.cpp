#include "engine/utilities/image-info/IMGDecoder.hpp"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <fstream>

#include "engine/core/defaults/sysfont_data.hpp"
#include "engine/debug/geng_debug.hpp"
#include "engine/scene/font/Font.hpp"
#include "engine/scene/initializer/Texture.hpp"
#include "engine/types/strings/hstring/hstring.hpp"

using namespace gan;

static bool is_PNG(gan::hstring path) {
    return true;
}

static long to_pow_2(const int x) {
    long i = 1;
    for (; i < x; i*=2){}
    return i;
}

IMG_Info IMGDecoder::PNG_Info(const char path[]) {
    if (!is_PNG(path)) {
        glog::err << "File " << path << "is not a png\n";
    }
    // Creates a file we read with binary from
    std::ifstream file(path, std::ios::binary);
    if (!file)
        glog::err.src("Is_PNG") << "FATAL: Image" << path << " does not exist.";
    uint8_t header[24];
    // Read from the file
    file.read(reinterpret_cast<char*>(header), 24);
    // check against the signature:
    static const uint8_t sig[8] =
        {137, 80, 78, 71, 13, 10, 26, 10};
    // actually we'll just use memcmp instead
    if (std::memcmp(header, sig, 8) != 0) {
        glog::err.src("IMGDecoder::png_info()") << "FATAL: " << path << " File signature does not match .png format";
    }
    // create a small function lambda that grabs the bits
    auto read_int = [](const uint8_t* p) {
        return static_cast<uint32_t>(to_pow_2((static_cast<uint32_t>(p[0]) << 24) |
                (static_cast<uint32_t>(p[1]) << 16) |
                (static_cast<uint32_t>(p[2]) << 8) |
                (static_cast<uint32_t>(p[3]) << 0)));
    };
    // create our image object with an initializer list.
    return {path, read_int(header + 16), read_int(header + 20)};
}


// Note: We rename load_ktx2_as_texture to the new function name load_image_as_texture
Texture IMGDecoder::load_image_as_texture(
    SDL_Renderer *renderer, hstring path) {


    SDL_Surface* src = IMG_Load(path.c_str());
    src = SDL_ConvertSurfaceFormat(src, SDL_PIXELFORMAT_RGBA8888, 0);

    int new_w = to_pow_2(src->w);
    int new_h = to_pow_2(src->h);
    SDL_Surface* padded = SDL_CreateRGBSurfaceWithFormat(
        0,
        new_w,
        new_h,
        32,
        SDL_PIXELFORMAT_RGBA8888
    );

    SDL_Rect dst = {0, 0, src->w, src->h};
    SDL_BlitSurface(src, nullptr, padded, &dst);

    Uint32 white = SDL_MapRGBA(padded->format, 255,255,255,255);
    ((Uint32*)padded->pixels)[(new_h - 1) * new_w + (new_w - 1)] = white;

    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, padded);
    SDL_FreeSurface(src);
    SDL_FreeSurface(padded);

    if (tex == nullptr) {
        glog::warn.src("IMGDecoder::load_image_as_texture()") << "Failed to load image file " << path << " using SDL_image." << glog::endlog;
        glog::err.src("IMGDecoder::load_image_as_texture()") << "SDL_image Error: " << IMG_GetError() << glog::endlog;
    }
    SDL_SetTextureScaleMode(tex, SDL_ScaleModeNearest);
    // This blend mode setting is often still required for transparency
    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);

    return {PNG_Info(path), tex};
}

static void get_font_width_height(Font& font, int& atlasW, int& atlasH) {
    constexpr int MAX_ATLAS_WIDTH = 512;
    const int padding = font.get_padding();

    atlasW = 0;
    atlasH = padding;

    int rowW = padding;
    int rowH = 0;

    for (int c = 32; c < 127; ++c) {
        SDL_Surface* s =
            TTF_RenderGlyph_Blended(font.get_font(), c, {255,255,255,255});
        if (!s) continue;

        if (rowW + s->w + padding > MAX_ATLAS_WIDTH) {
            atlasW = std::max(atlasW, rowW);
            atlasH += rowH + padding;
            rowW = padding;
            rowH = 0;
        }

        rowW += s->w + padding;
        rowH = std::max(rowH, s->h);

        SDL_FreeSurface(s);
    }

    atlasW = std::max(atlasW, rowW);
    atlasH += rowH + padding;
}

Texture IMGDecoder::load_font_as_texture(SDL_Renderer* renderer,
                                         Font& font,
                                         hstring path)
{
    const int padding = font.get_padding();

    int atlasW = 0, atlasH = 0;
    get_font_width_height(font, atlasW, atlasH);

    SDL_Surface* atlasSurface =
        SDL_CreateRGBSurfaceWithFormat(
            0, atlasW, atlasH, 32, SDL_PIXELFORMAT_RGBA32);

    if (!atlasSurface) {
        glog::err << "Failed to create atlas surface: "
                  << SDL_GetError() << glog::endlog;
        return {};
    }

    // Clear atlas to transparent
    SDL_FillRect(
        atlasSurface,
        nullptr,
        SDL_MapRGBA(atlasSurface->format, 0, 0, 0, 0)
    );

    int penX = padding;
    int penY = padding;
    int rowH = 0;

    for (int c = 32; c < 127; ++c) {
        SDL_Surface* glyphSurface =
            TTF_RenderGlyph_Solid(
                font.get_font(),
                c,
                SDL_Color{255, 255, 255, 255}
            );

        if (!glyphSurface)
            continue;

        // Wrap to next row if needed
        if (penX + glyphSurface->w + padding > atlasW) {
            penX = padding;
            penY += rowH + padding;
            rowH = 0;
        }

        SDL_Rect dst{
            penX,
            penY,
            glyphSurface->w,
            glyphSurface->h
        };

        SDL_BlitSurface(glyphSurface, nullptr, atlasSurface, &dst);

        // Glyph metrics (used for advance & bearings during text rendering)
        int minx, maxx, miny, maxy, advance;
        TTF_GlyphMetrics(font.get_font(), c,
                         &minx, &maxx, &miny, &maxy, &advance);

        /// Add the character to our font.
        /// NOTE: dst is atlas position; advance is logical spacing.
        font.add_char(
            c,
            { atlasW, atlasH },
            { dst.x, dst.y },
            { dst.w, dst.h },
            advance
        );

        penX += dst.w + padding;
        rowH  = std::max(rowH, dst.h);

        SDL_FreeSurface(glyphSurface);
    }

    Texture atlas{};
    Uint32 white = SDL_MapRGBA(atlasSurface->format, 255,255,255,255);
    ((Uint32*)atlasSurface->pixels)[(atlasSurface->h - 1) * atlasSurface->w + (atlasSurface->w - 1)] = white;
    atlas.texture = SDL_CreateTextureFromSurface(renderer, atlasSurface);

    if (!atlas.texture) {
        glog::err << "Failed to create atlas texture: "
                  << SDL_GetError() << glog::endlog;
        SDL_FreeSurface(atlasSurface);
        return {};
    }

    SDL_SetTextureBlendMode(atlas.texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureScaleMode(atlas.texture, SDL_ScaleModeNearest);

    // IMPORTANT: store an owning string, NOT c_str() from a temporary
    atlas.info.filename = path.c_str();
    atlas.info.w = atlasW;
    atlas.info.h = atlasH;

    SDL_FreeSurface(atlasSurface);
    return atlas;
}

Texture IMGDecoder::create_sysfont_texture(SDL_Renderer *renderer) {

    SDL_Texture* tex = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STATIC,
        SYSFONT_W,
        SYSFONT_H
    );

    SDL_UpdateTexture(
        tex,
        nullptr,
        SYSFONT_DATA,
        SYSFONT_W * 4
    );

    SDL_SetTextureBlendMode(tex, SDL_BLENDMODE_BLEND);
    SDL_SetTextureScaleMode(tex, SDL_ScaleModeNearest);

    Texture atlas;
    atlas.texture = tex;
    atlas.info.w = SYSFONT_W;
    atlas.info.h = SYSFONT_H;
    atlas.info.filename = "__sysfont";
    return atlas;
}