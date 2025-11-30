#pragma once
#include "../../gengine-globals/EngineEnums.hpp"

struct EngineElement {
    gengine::GENG_Type type;
    float z_index;
    int id;
    void* target;
    EngineElement(gengine::GENG_Type t, const float z, int id, void* tg)
        : type(t), z_index(z), id(id), target(tg) {}
};