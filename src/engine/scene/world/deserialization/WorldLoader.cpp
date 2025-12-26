#include "engine/scene/world/deserialization/WorldLoader.hpp"

#include <fstream>

#include "../../../../../include/engine/debug/geng_debug.hpp"
#include "engine/scene/world/GameWorld.hpp"
#include "engine/scene/world/WorldProperties.hpp"
#include "engine/utilities/Utilities.hpp"
using namespace geng;

// --- Binary helpers ---
template<typename T>
void write_binary(std::ofstream& out, const T& val) {
    out.write(reinterpret_cast<const char*>(&val), sizeof(T));
}

template<typename T>
T read_binary(std::ifstream& in) {
    T val;
    in.read(reinterpret_cast<char*>(&val), sizeof(T));
    return val;
}

gch::vector<char> read_char_vector(std::ifstream& in) {
    auto len = read_binary<uint32_t>(in);
    gch::vector<char> v(len);
    if(len) in.read(v.data(), len);
    return v;
}

gch::vector<uint16_t> read_uint16_vector(std::ifstream& in) {
    auto len = read_binary<uint32_t>(in);
    gch::vector<uint16_t> v(len);
    if(len) in.read(reinterpret_cast<char*>(v.data()), len * sizeof(uint16_t));
    return v;
}

TProperty read_property(std::ifstream& in) {
    TProperty prop;
    gch::vector<char> name_chars = read_char_vector(in);
    prop.name = hstring(name_chars.data(), name_chars.size());
    prop.value = read_binary<int32_t>(in);
    return prop;
}
GameWorld WorldLoader::read_world(hstring filename) {
    std::ifstream in(filename, std::ios::binary);
    if(!in.is_open()) {
        glog::note << "Failed to open file: " << filename << glog::endlog;
        abort();
    }

    GameWorld world;
    world.magic = read_binary<uint64_t>(in);
    world.num_levels = read_binary<uint16_t>(in);
    world.levels.resize(world.num_levels);

    for (auto& lvl : world.levels) {
        // --- Read header in the order it was written ---
        uint16_t num_layers = read_binary<uint16_t>(in);
        lvl.width = read_binary<uint32_t>(in);
        lvl.height = read_binary<uint32_t>(in);
        lvl.xOffset = read_binary<int32_t>(in);
        lvl.yOffset = read_binary<int32_t>(in);
        glog::dev << "\nCreating level:\n";
        glog::dev << "\tNUMLAYERS: " << num_layers << "\n";
        glog::dev << "\tWIDTH: " << lvl.width << "\n";
        glog::dev << "\tHEIGHT: " << lvl.height << "\n";
        glog::dev << "\tXOFFSET: " << lvl.xOffset << "\n";
        glog::dev << "\tYOFFSET: " << lvl.yOffset << glog::endlog;
        // Resize layers vector according to num_layers
        lvl.layers.resize(num_layers);

        for (auto& layer : lvl.layers) {
            layer.layerClass = static_cast<TLayer::Class>(read_binary<uint8_t>(in));
            layer.visible = read_binary<uint8_t>(in);

            if (layer.layerClass != TLayer::Class::OBJ) {
                layer.data = read_uint16_vector(in);
            } else {
                auto num_objects = read_binary<uint32_t>(in);
                layer.objects.resize(num_objects);

                for (auto& obj : layer.objects) {
                    obj.id = read_binary<uint32_t>(in);
                    gch::vector<char> temp = read_char_vector(in);
                    obj.templateName = hstring(temp.data(), temp.size());
                    obj.x = read_binary<float>(in);
                    obj.y = read_binary<float>(in);

                    auto num_props = read_binary<uint32_t>(in);
                    obj.properties.resize(num_props);

                    for (auto& prop : obj.properties)
                        prop = read_property(in);
                }
            }
        }
    }

    return world;
}