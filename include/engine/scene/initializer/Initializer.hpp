#pragma once

#include "TextureRegister.hpp"
#include "engine/scene/animation//FrameList.hpp"
#include "engine/scene/animation/FrameTable.hpp"
#include "engine/scene/font/Font.hpp"
#include "engine/scene/font/FontList.hpp"
#include "engine/scene/tilesets/TileList.hpp"
#include "engine/scene/tilesets/Tileset.hpp"

namespace gan {
    /**
     * The initializer takes care of making sure object definitions get to the right place, and that textures are initialized
     */
    class Initializer final {
        /// The layerContext object
        LayerContext& scene;
        /// Allows the initializer to add frametables to the layer's framelist.
        FrameList& frames;
        /// Contains all the tilesets that will be added to the layer's tilesetlist
        TileList& tiles;
        /// Contains all the textures that will need to be created
        TextureRegister& texreg;

    public:
        explicit Initializer(LayerContext& scene, FrameList& frames, TileList& tiles, TextureRegister& tex_reg);
        /// Texture
        int texture(hstring path);
        /// Sets up a FrameTable and its corresponding texture to be initialized. Returns the ID of the frameTable
        int frame_table(hstring path, FrameTable ft);
        /// Sets up a Tileset and it's corresponding texture
        int tileset(hstring path, Tileset t);
    };
}
