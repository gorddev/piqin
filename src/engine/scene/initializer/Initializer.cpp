#include "engine/scene/initializer/Initializer.hpp"

#include "engine/scene/initializer/system-assets/sys-font.hpp"
#include "engine/utilities/image-info/IMGDecoder.hpp"

namespace geng {
    class FontList;
    class FrameManager;
}

using namespace geng;

Initializer::Initializer(TextureRegister& tex_reg, LayerContext& scene) : tex_reg(tex_reg), scene(scene) {
    std::cerr << "making initiazlier\n";
    font("./sys-font.png", sys_font);

}

int Initializer::texture(const std::string &path, bool _internal) {
    if (tex_reg.has_texture(path)) {
        if (!_internal)
            scene.log(debug::Severity::WARNING, "Texture " + path +  "already loaded.", "Initializer::texture()");
        return tex_reg.get_id(path);
    }
    scene.log(0, "Calling texture register\n", "geng::Initializer::texture");
    return tex_reg.register_texture(path);
}

int Initializer::frame_table(const std::string &path, FrameTable ft) {
    // Sets texture id of the frametable
    ft.set_texture_id(texture(path, true));
    frameTables.emplace_back(ft);
    return frameTables.size() + (table_num++);
}

int Initializer::font(const std::string &path, Font f, int id) {
    tex_reg.size();
    f.set_texture_id(texture(path, true));
    fonts.emplace_back(f);
    return fonts.size() + (font_num++);
}


void Initializer::set_sys_font(const std::string &path, const Font &fnt) {
    // First we destroy the original system font texture.
    int old_id = tex_reg.path_to_textureID["./sys-font.png"];
    tex_reg.destroy_texture(old_id);

    // Update our 0th font
    fonts[0] = fnt;
    // Assign the new texture ID to the user-provided font
    fonts[0].set_texture_id(texture(path, true));

    // Now we search and update any fonts that were using the old texture
    for (auto& font_obj : fonts) {
        if (font_obj.get_texture_id() == old_id) {
            scene.log(1, "Warning: updating texture ID of font. "
                          "Please call set_sys_font before loading textures for maximum safety.\n",
                          "geng::Initializer::set_sys_font()");
            font_obj.set_texture_id(fnt.get_texture_id());
        }
    }

    // Update any frameTables that were using the old texture
    for (auto& ft : frameTables) {
        if (ft.get_texture_id() == old_id) {
            scene.log(1, "Updating texture ID of frameTable. "
                          "Please call set_sys_font before loading textures for maximum safety.\n",
                          "geng::Initializer::set_sys_font()");
            ft.set_texture_id(fnt.get_texture_id());
        }
    }
    // Finally, update the TextureRegister mapping for the new font path
    tex_reg.path_to_textureID[path] = fnt.get_texture_id();
}

void Initializer::_compose(FrameManager &fm, FontList &fl) {
    // stores the dimensions of each of our texutures
    std::vector<IMG_Info> dimensions;
    dimensions.reserve(tex_reg.size());
    for (int i = 0; i < tex_reg.path_to_textureID.size(); i++)
        dimensions.emplace_back("", 0, 0);

    // grabs the dimensions of each of the textures
    std::cerr << "number of textures: " << tex_reg.path_to_textureID.size() << "\n";
    for (auto& [str, i]: tex_reg.path_to_textureID) {
        if (i > dimensions.size() || i < 0) {
            scene.log(debug::Severity::ERROR, "Dimension mismatch. Check sys_font initialization.\n", "Initializer::_compose()");
            abort();
        }
        // Grabs our image information.
        dimensions[i] = IMGDecoder::PNG_Info(str);
    }

    // Initializes all frameTables
    for (auto& f: frameTables) {
        int id = f.get_texture_id();
        f._init(dimensions[id]);
    }
    // Initializes all fonts
    for (auto& f: fonts) {
        int id = f.get_texture_id();
        f._init(dimensions[id]);
    }
    // Sets up the frame_manager with all the required goodies
    fm.add_tables(frameTables);
    // sets up the font_list with all the required goodies
    fl.add_fonts(fonts);

    _clear();
}

void Initializer::_clear() {
    frameTables = {};
    fonts = {};
}

