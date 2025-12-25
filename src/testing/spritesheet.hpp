#pragma once

#include "engine/scene/animation/FrameTable.hpp"

inline float basedur = 120.f;

const geng::FrameTable testsprite = {
    {
        {
            //anim 0
            {0, 0, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32, 0, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*2, 0, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*3, 0, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*4, 0, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*5, 0, 32, 32, basedur, geng::GAnimType::REPEAT}
        },
        {   //anim 1
            {32*0, 32, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*1, 32, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*2, 32, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*3, 32, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*4, 32, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*5, 32, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*6, 32, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*7, 32, 32, 32, basedur, geng::GAnimType::REPEAT}
        },
        {   //anim 2
            {32*0, 64, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*1, 64, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*2, 64, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*3, 64, 32, 32, basedur, geng::GAnimType::RESET}
        },
        { // anim 3
            {32*0, 32*4, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*1, 32*4, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*2, 32*4, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*3, 32*4, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*4, 32*4, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*5, 32*4, 32, 32, basedur, geng::GAnimType::RESET, 4}
        },
        { // anim 4
            {32*0, 32*5, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*1, 32*5, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*2, 32*5, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*3, 32*5, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*4, 32*5, 32, 32, basedur, geng::GAnimType::CONTINUE},
            {32*5, 32*5, 32, 32, basedur, geng::GAnimType::REPEAT}
        }
    }
};