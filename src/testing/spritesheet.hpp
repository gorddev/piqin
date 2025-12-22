#pragma once

#include "engine/scene/animation/FrameTable.hpp"

inline float basedur = 120.f;

const geng::FrameTable testsprite = {
    {
        {
            //anim 1
            {0, 0, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32, 0, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*2, 0, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*3, 0, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*4, 0, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*5, 0, 32, 32, basedur, geng::GAnimType::REPEAT}
        },
        {   //anim 2
            {32*0, 32, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*1, 32, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*2, 32, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*3, 32, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*4, 32, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*5, 32, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*6, 32, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*7, 32, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*8, 32, 32, 32, basedur, geng::GAnimType::REPEAT},
        }
    }
};