#include "engine/scene/world/GameWorld.hpp"

#include <iomanip>
#include <sstream>

#include "engine/rendering/Camera.hpp"
#include "engine/scene/tilesets/TileBuffer.hpp"

using namespace geng;

GameLevel* GameWorld::get_first_level() {
    return &levels[0];
}

GameLevel* GameWorld::search_for_level(int x, int y) {
    for (auto&l : levels) {
        if (x >= l.xOffset && x < l.xOffset + l.width
            && y >= l.yOffset && y < l.yOffset + l.height)
            return &l;
    }
    return nullptr;
}

int GameWorld::worldWidth() {
    int max = 0;
    for (auto & l : levels) {
        if (l.xOffset + l.width > max)
            max = l.xOffset + l.width;
    }
    return max;
}

int GameWorld::worldHeight() {
    int max = 0;
    for (auto & l : levels) {
        if (l.yOffset + l.height > max)
            max = l.yOffset + l.height;
    }
    return max;
}

void GameWorld::render_world(geng::TileBuffer &buffer, geng::Camera &cam) {
    // Get camera bounds
    float camX = cam.pos.x;
    float camY = cam.pos.y;
    float camW = cam.get_width();
    float camH = cam.get_height();

    int tileSize = buffer.get_tile_size();

    // Compute visible tile range
    int startCol = static_cast<int>(camX / tileSize);
    int endCol   = static_cast<int>((camX + camW) / tileSize);

    // Iterate levels
    int count = 0;
    for (const auto &lvl : levels) {
        if (lvl.xOffset > cam.right()) continue;
        if (lvl.xOffset + lvl.width*tileSize < cam.pos.x) continue;
        if (lvl.yOffset > cam.bottom()) continue;
        if (lvl.yOffset + lvl.height*tileSize < cam.pos.y) continue;

        // Iterate layers
        for (const auto &layer : lvl.layers) {
            if (!layer.visible || layer.layerClass == TLayer::Class::OBJ) continue;
            // Iterate only visible tiles
            for (int col = 0; col < lvl.width; col++) {
                for (int row = 0; row < lvl.height; row++) {
                    int index = layer.data[row * lvl.width + col];
                    if (index != 0 && index < layer.data.size()) {
                        Pos2D tilePos{ (col * tileSize) + lvl.xOffset,
                                             (row*tileSize) + lvl.yOffset
                            };
                        buffer.push_back(index - 1, tilePos);
                    }
                }
            }
        }
        count++;
    }
}

static const char* layer_class_to_string(TLayer::Class c) {
    switch (c) {
        case TLayer::Class::VIS:     return "VIS";
        case TLayer::Class::PHYSVIS: return "PHYSVIS";
        case TLayer::Class::OBJ:     return "OBJ";
        default:                        return "UNKNOWN";
    }
}

std::string GameWorld::to_string() const {
    std::ostringstream out;

    out << "=== GameWorld ===\n";
    out << "Magic: 0x" << std::hex << magic << std::dec << "\n";
    out << "Num Levels: " << num_levels << "\n\n";

    for (size_t li = 0; li < levels.size(); ++li) {
        const GameLevel& lvl = levels[li];

        out << "Level [" << li << "]\n";
        out << "  Size: " << lvl.width << " x " << lvl.height << "\n";
        out << "  Offset: (" << lvl.xOffset << ", " << lvl.yOffset << ")\n";
        out << "  Num Layers: " << lvl.layers.size() << "\n";

        for (size_t ly = 0; ly < lvl.layers.size(); ++ly) {
            const TLayer& layer = lvl.layers[ly];

            out << "    Layer [" << ly << "]\n";
            out << "      Class: " << layer_class_to_string(layer.layerClass) << "\n";
            out << "      Visible: " << (layer.visible ? "true" : "false") << "\n";

            if (layer.layerClass == TLayer::Class::OBJ) {
                out << "      Objects: " << layer.objects.size() << "\n";
                for (size_t oi = 0; oi < layer.objects.size(); ++oi) {
                    const TObject& obj = layer.objects[oi];
                    out << "        Object [" << oi << "] id=" << obj.id
                        << " template=" << obj.templateName
                        << " pos=(" << obj.x << "," << obj.y << ")\n";
                    if (!obj.properties.empty()) {
                        out << "          Properties:\n";
                        for (const auto& prop : obj.properties) {
                            out << "            " << prop.name << " = " << prop.value << "\n";
                        }
                    }
                }
            } else {
                out << "      Tiles: " << layer.data.size() << "\n";
                out << "      Tile Grid:\n";
                for (uint32_t y = 0; y < lvl.height; ++y) {
                    out << "        ";
                    for (uint32_t x = 0; x < lvl.width; ++x) {
                        uint32_t idx = y * lvl.width + x;
                        uint16_t t = (idx < layer.data.size()) ? layer.data[idx] : 0;
                        out << std::setw(4) << t << " ";
                    }
                    out << "\n";
                }
            }
        }
        out << "\n";
    }
    return out.str();
}