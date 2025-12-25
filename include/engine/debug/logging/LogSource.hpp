#pragma once
#include "LogBucket.hpp"

namespace glog {

    inline geng::logcommands endlog = geng::logcommands::STOP;

    inline geng::debug::LogBucket<300, geng::debug::DEBUG> dev;
    inline geng::debug::LogBucket<300, geng::debug::INFO> note;
    inline geng::debug::LogBucket<40, geng::debug::WARNING> warn;
    inline geng::debug::LogBucket<10, geng::debug::ERROR> err;

}
