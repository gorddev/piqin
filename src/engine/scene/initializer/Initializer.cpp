#include "engine/scene/initializer/Initializer.hpp"

#include "engine/scene/tilesets/TileList.hpp"
#include "engine/utilities/image-info/IMGDecoder.hpp"
#include "engine/core/defaults/sysfont.inl"

namespace geng {
    class FontList;
    class FrameList;
}

using namespace geng;

Initializer::Initializer(LayerContext &scene,
        FrameList &frames, FontList &fonts, TileList &tiles,
        TextureRegister &tex_reg) : scene(scene), frames(frames), fonts(fonts), tiles(tiles), texreg(tex_reg)
{
    font("./assets/sys-font.png", sys_font);
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

int Initializer::font(hstring path, Font f, int id) {
    f.set_texture_id(texture(path));
    IMG_Info i = IMGDecoder::PNG_Info(path);
    glog::note.src("Initializer::font") << "Image information for font: w: " << i.w << " h: " << i.h << " fn" << i.filename << " texid: " << f.get_texture_id() << glog::endlog;
    f._init(i);
    return fonts.add_font(f);
}

int Initializer::tileset(hstring path, Tileset t) {
    t.set_texture_id(texture(path));
    return tiles.add_tileset(t);
}
