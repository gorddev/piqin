#pragma once
#include "../../gengine-globals/EngineEnums.hpp"
#include "../../objects/Object.hpp"
#include "engine/particles/ParticleGroup.hpp"

namespace gengine {
    struct EngineElement {
        gengine::GENG_Type type;
        float* z_index;
        int id;
        void* target;
        EngineElement(gengine::GENG_Type t, const float z, int id, void* tg)
            : type(t), id(id), target(tg) {
            if (type == gengine::GENG_Type::OBJECT)
                z_index = static_cast<Object*>(tg)->zptr();
            else if (type == gengine::GENG_Type::PARTICLE)
                z_index = &static_cast<ParticleGroup*>(tg)->pos.z;
            else
                std::cerr << "[FATAL] Non-OBJECT or PARTICLE EngineElement Created.\n";
        }
    };
}
