#pragma once
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

namespace geng {

    /// Singular property of an object passed through the parser
    struct TProperty {
        std::string name;
        int32_t value;
    };

    /// An object containing a list of properties
    struct TObject {
        uint32_t id;
        std::string templateName;
        float x = 0.f, y = 0.f;
        std::vector<TProperty> properties;
    };

    /// One layer, which can contain either tiles or objects.
    struct TLayer {
        enum class Class { VIS, PHYSVIS, OBJ } layerClass;
        std::vector<uint16_t> data;     // tile data
        std::vector<TObject> objects;    // only used for OBJ layers
        bool visible = true;
    };

    /// One gameLevel, which contains Layers consisting of either tiles or objects, ordered by z.
    struct GameLevel {
        std::string fileName;
        int xOffset = 0;
        int yOffset = 0;
        int width = 0;
        int height = 0;

        uint32_t tileWidth = 0;
        uint32_t tileHeight = 0;
        std::vector<std::string> tilesets;

        std::vector<TLayer> layers;

        // Grabs any physics layers from the level
        std::vector<uint16_t>& extract_physics_data() {
            for (auto& i: layers) {
                if (i.layerClass == TLayer::Class::PHYSVIS) {
                    return i.data;
                }
            }
            std::cerr << "Fatal: Could not find physics layer for level: " + fileName + "\n";
            abort();
        }
    };
}