#include "engine/scene/initializer/Initializer.hpp"

#include "engine/scene/tilesets/TileList.hpp"
#include "engine/utilities/image-info/IMGDecoder.hpp"
#include "../../../../include/engine/defaults/sysfont.inl"

namespace geng {
    class FontList;
    class FrameList;
}

using namespace geng;

Initializer::Initializer(LayerContext &scene,
        FrameList &frames, FontList &fonts, TileList &tiles,
        TextureRegister &tex_reg) : scene(scene), frames(frames), fonts(fonts), tiles(tiles), texreg(tex_reg)
{
    glog::note.src("Initializer::Initializer") << "Attempting to create initializer. Starting with sys_font initialization." << glog::endlog;
    font("./assets/sys-font.png", sys_font);
    glog::note.src("Initializer::Initializer") << "Sysfont created without a crash" << glog::endlog;
}

int Initializer::texture(hstring path) {
    return texreg.register_texture(path);
}

int Initializer::frame_table(hstring path, FrameTable ft) {
    // Sets texture id of the frametable
    IMG_Info i = IMGDecoder::PNG_Info(path);
    ft._init(i);
    ft.set_texture_id(texture(path));
    glog::note.src("Initializer::frame_table")
        << "Making frametable " << path.cstr() << " with id "<< ft.get_texture_id()
        << glog::endlog;
    return frames.add_table(ft);
}

int Initializer::font(hstring path, Font f, int id) {
    glog::warn << "Attempting to set up texture to be rendered for" << path.cstr() << glog::endlog;
    f.set_texture_id(texture(path));
    glog::warn << "Now trying to load the png info of " << path.cstr() << glog::endlog;
    f._init(IMGDecoder::PNG_Info(path));
    glog::note.src("Initializer::font") << "Making font " << path.cstr() << " with id " << id
        << glog::endlog;
    return fonts.add_font(f);
}

int Initializer::tileset(hstring path, Tileset t) {
    t.set_texture_id(texture(path));
    glog::note.src("Initializer::tileset") << "Making tileset " << path.cstr()
        << " with id " << t.get_texture_id() << glog::endlog;
    return tiles.add_tileset(t);
}
