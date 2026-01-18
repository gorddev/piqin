#include "engine/scene/initializer/Initializer.hpp"

#include "engine/scene/tilesets/TileList.hpp"
#include "engine/utilities/image-info/IMGDecoder.hpp"

namespace gan {
    class FontList;
    class FrameList;
}

using namespace gan;

Initializer::Initializer(LayerContext &scene,
        FrameList &frames, TileList &tiles,
        TextureRegister &tex_reg) : scene(scene), frames(frames), tiles(tiles), texreg(tex_reg) {

}

int Initializer::texture(hstring path) {
    return texreg.register_texture(path);
}

int Initializer::frame_table(hstring path, FrameTable ft) {
    // Sets texture id of the frametable
    IMG_Info i = IMGDecoder::PNG_Info(path);
    ft._init(i);
    ft.set_texture_id(texture(path));
    return frames.add_table(ft);
}

int Initializer::tileset(hstring path, Tileset t) {
    t.set_texture_id(texture(path));
    return tiles.add_tileset(t);
}
