#include <fstream>

#include "../../../../../.protected/json-importer/json-helpers/json.hpp"

using json = nlohmann::json;

// this is not my code :o

// --- TProperty ---
struct TProperty {
    std::string name;
    int32_t value;
};

// --- TObject ---
struct TObject {
    uint32_t id;
    std::string templateName;
    float x = 0.f, y = 0.f;
    lni::vector<TProperty> properties;
};

// --- Layer ---
struct TLayer {
    enum class Class { VIS, PHYSVIS, OBJ } layerClass;
    lni::vector<uint16_t> data;     // tile data
    lni::vector<TObject> objects;    // only used for OBJ layers
    bool visible = true;
};

// --- GameLevel ---
struct GameLevel {
    uint16_t num_layers;
    uint32_t width, height;
    int32_t xOffset = 0, yOffset = 0;
    lni::vector<TLayer> layers;
};

// --- GameWorld ---
struct GameWorld {
    uint64_t magic;
    uint16_t num_levels;
    lni::vector<GameLevel> levels;
};

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

void write_char_vector(std::ofstream& out, const lni::vector<char>& v) {
    uint32_t len = static_cast<uint32_t>(v.size());
    write_binary(out, len);
    if(len) out.write(v.data(), len);
}

lni::vector<char> read_char_vector(std::ifstream& in) {
    uint32_t len = read_binary<uint32_t>(in);
    lni::vector<char> v(len);
    if(len) in.read(v.data(), len);
    return v;
}

void write_uint16_vector(std::ofstream& out, const lni::vector<uint16_t>& v) {
    uint32_t len = static_cast<uint32_t>(v.size());
    write_binary(out, len);
    if(len) out.write(reinterpret_cast<const char*>(v.data()), len * sizeof(uint16_t));
}

lni::vector<uint16_t> read_uint16_vector(std::ifstream& in) {
    uint32_t len = read_binary<uint32_t>(in);
    lni::vector<uint16_t> v(len);
    if(len) in.read(reinterpret_cast<char*>(v.data()), len * sizeof(uint16_t));
    return v;
}

// --- Serialize/deserialize TProperty ---
void write_property(std::ofstream& out, const TProperty& prop) {
    write_char_vector(out, lni::vector<char>(prop.name.begin(), prop.name.end()));
    write_binary(out, prop.value);
}

TProperty read_property(std::ifstream& in) {
    TProperty prop;
    lni::vector<char> name_chars = read_char_vector(in);
    prop.name = std::string(name_chars.begin(), name_chars.end());
    prop.value = read_binary<int32_t>(in);
    return prop;
}

// --- Parse JSON into TObject ---
TObject parse_object(const json& obj_json) {
    TObject obj;
    obj.id = obj_json["id"];
    obj.templateName = obj_json.value("template", "");
    obj.x = obj_json["x"];
    obj.y = obj_json["y"];

    if(obj_json.contains("properties")) {
        for(auto& prop_json : obj_json["properties"]) {
            TProperty prop;
            prop.name = prop_json["name"];
            prop.value = prop_json["value"];
            obj.properties.push_back(prop);
        }
    }
    return obj;
}

// --- Parse JSON into Layer ---
TLayer parse_layer(const json& layer_json) {
    TLayer layer;
    std::string cls = layer_json["class"];
    if(cls == "vis") layer.layerClass = TLayer::Class::VIS;
    else if(cls == "physvis") layer.layerClass = TLayer::Class::PHYSVIS;
    else if(cls == "obj") layer.layerClass = TLayer::Class::OBJ;
    else throw std::runtime_error("Unknown layer class: " + cls);

    layer.visible = layer_json.value("visible", true);

    if(layer.layerClass != TLayer::Class::OBJ) {
        for(auto& gid : layer_json["data"])
            layer.data.push_back(gid.get<uint16_t>());
    } else {
        for(auto& obj_json : layer_json["objects"])
            layer.objects.push_back(parse_object(obj_json));
    }
    return layer;
}

// --- Parse JSON into GameLevel ---
GameLevel parse_level(const std::string& filename, int32_t xOffset = 0, int32_t yOffset = 0, int wOverride = 0, int hOverride = 0) {
    std::ifstream file(filename);
    if(!file.is_open()) throw std::runtime_error("Failed to open level file: " + filename);

    json j;
    file >> j;

    GameLevel lvl;

    lvl.width = (wOverride) ? wOverride : j["width"];
    lvl.height = (hOverride) ? hOverride : j["height"];
    lvl.xOffset = xOffset;
    lvl.yOffset = yOffset;

    for(auto& layer_json : j["layers"])
        lvl.layers.push_back(parse_layer(layer_json));

    lvl.num_layers = static_cast<uint16_t>(lvl.layers.size());
    return lvl;
}

// --- Write/Read Layer/GameLevel/GameWorld ---
void write_world(const GameWorld& world, const std::string& outputfile) {
    std::ofstream out(outputfile, std::ios::binary);
    if(!out.is_open()) throw std::runtime_error("Failed to open output file: " + outputfile);

    write_binary(out, world.magic);
    write_binary(out, world.num_levels);

    for(const auto& lvl : world.levels) {
        write_binary(out, lvl.num_layers);
        write_binary(out, lvl.width);
        write_binary(out, lvl.height);
        write_binary(out, lvl.xOffset);
        write_binary(out, lvl.yOffset);

        for(const auto& layer : lvl.layers) {
            write_binary(out, static_cast<uint8_t>(layer.layerClass));
            write_binary(out, layer.visible);

            if(layer.layerClass != TLayer::Class::OBJ)
                write_uint16_vector(out, layer.data);
            else {
                uint32_t num_objects = static_cast<uint32_t>(layer.objects.size());
                write_binary(out, num_objects);
                for(const auto& obj : layer.objects) {
                    write_binary(out, obj.id);
                    write_char_vector(out, lni::vector<char>(obj.templateName.begin(), obj.templateName.end()));
                    write_binary(out, obj.x);
                    write_binary(out, obj.y);

                    // write properties
                    uint32_t num_props = static_cast<uint32_t>(obj.properties.size());
                    write_binary(out, num_props);
                    for(const auto& prop : obj.properties)
                        write_property(out, prop);
                }
            }
        }
    }
}

GameWorld read_world(const std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if(!in.is_open()) throw std::runtime_error("Failed to open file: " + filename);

    GameWorld world;
    world.magic = read_binary<uint64_t>(in);
    world.num_levels = read_binary<uint16_t>(in);
    world.levels.resize(world.num_levels);

    for(auto& lvl : world.levels) {
        lvl.num_layers = read_binary<uint16_t>(in);
        lvl.width = read_binary<uint32_t>(in);
        lvl.height = read_binary<uint32_t>(in);
        lvl.xOffset = read_binary<int32_t>(in);
        lvl.yOffset = read_binary<int32_t>(in);

        lvl.layers.resize(lvl.num_layers);
        for(auto& layer : lvl.layers) {
            layer.layerClass = static_cast<TLayer::Class>(read_binary<uint8_t>(in));
            layer.visible = read_binary<uint8_t>(in);

            if(layer.layerClass != TLayer::Class::OBJ)
                layer.data = read_uint16_vector(in);
            else {
                uint32_t num_objects = read_binary<uint32_t>(in);
                layer.objects.resize(num_objects);
                for(auto& obj : layer.objects) {
                    obj.id = read_binary<uint32_t>(in);
                    lni::vector<char> temp = read_char_vector(in);
                    obj.templateName = std::string(temp.begin(), temp.end());
                    obj.x = read_binary<float>(in);
                    obj.y = read_binary<float>(in);

                    // read properties
                    uint32_t num_props = read_binary<uint32_t>(in);
                    obj.properties.resize(num_props);
                    for(auto& prop : obj.properties)
                        prop = read_property(in);
                }
            }
        }
    }
    return world;
}