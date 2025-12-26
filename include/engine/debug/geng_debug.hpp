#pragma once
#include "engine/debug/logging/LogBucket.hpp"
#include "engine/debug/logging/LogStream.hpp"

namespace geng {
    /// Current debug state
    inline bool debug_mode = true;
    /// Current console severity
    inline debug::Severity report_level = debug::ERROR;
}

namespace glog {

    /// Command for ending a single log
    inline geng::logcommands endlog = geng::logcommands::ENDLOG;

    /// The stream that allows for raw user output into the console. Limtied to 2000 characters per output.
    inline geng::debug::LogStream<2000, geng::debug::DEBUG> dev;
    /// Create a note in the debug console.
    inline geng::debug::LogBucket<300, geng::debug::INFO> note;
    /// Create a warning in the debug console
    inline geng::debug::LogBucket<40, geng::debug::WARNING> warn;
    /// Call an error and crash the program, entering recovery mode.
    inline geng::debug::LogBucket<10, geng::debug::ERROR> err;

}
