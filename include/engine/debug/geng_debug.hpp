#pragma once
#include "engine/debug/logging/LogBucket.hpp"
#include "engine/debug/logging/LogStream.hpp"

namespace gan {
    /// Current debug state
    inline bool debug_mode = true;
    /// Current console severity
    inline debug::Severity report_level = debug::ERROR;
}

namespace glog {

    /// Command for ending a single log
    inline gan::logcommands endlog = gan::logcommands::ENDLOG;

    /// The stream that allows for raw user output into the console. Limtied to 2000 characters per output.
    inline gan::debug::LogStream<2000, gan::debug::DEBUG> dev;
    /// Create a note in the debug console.
    inline gan::debug::LogBucket<300, gan::debug::INFO> note;
    /// Create a warning in the debug console
    inline gan::debug::LogBucket<40, gan::debug::WARNING> warn;
    /// Call an error and crash the program, entering recovery mode.
    inline gan::debug::LogBucket<10, gan::debug::ERROR> err;

}
