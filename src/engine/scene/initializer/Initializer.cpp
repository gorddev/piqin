#include "engine/scene/initializer/Initializer.hpp"

#include "engine/mods/plugins/platformer/tilesets/TileList.hpp"
#include "engine/utilities/image-info/IMGDecoder.hpp"

namespace gan {
    class FontList;
    class FrameList;
}

using namespace gan;

Initializer::Initializer(LayerCore &scene, TileList &tiles,
        TextureRegister &tex_reg) : scene(scene), tiles(tiles), texreg(tex_reg) {}

int Initializer::texture(hstring path) {
    return get_tex_register().register_texture(path);
}


int Initializer::tileset(hstring path, Tileset t) {
    t.set_texture_id(texture(path));
    return tiles.add_tileset(t);
}
