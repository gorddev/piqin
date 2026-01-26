#pragma once

#include "TextureRegister.hpp"
#include "engine/layers/layer-subobjects/LayerCore.hpp"
#include "../../mods/animation/frames/FrameTable.hpp"
#include "engine/mods/plugins/platformer/tilesets/TileList.hpp"
#include "engine/mods/plugins/platformer/tilesets/Tileset.hpp"

namespace gan {
    /**
     * The initializer takes care of making sure object definitions get to the right place, and that textures are initialized
     */
    class Initializer final {
        /// The layerContext object
        LayerCore& scene;
        /// Contains all the tilesets that will be added to the layer's tilesetlist
        TileList& tiles;
        /// Contains all the textures that will need to be created
        TextureRegister& texreg;

    public:
        explicit Initializer(LayerCore& scene, TileList& tiles, TextureRegister& tex_reg);
        /// Texture
        int texture(hstring path);
        /// Sets up a FrameTable and its corresponding texture to be initialized. Returns the ID of the frameTable
        int frame_table(hstring path, FrameTable ft);
        /// Sets up a Tileset and it's corresponding texture
        int tileset(hstring path, Tileset t);
    };
}
