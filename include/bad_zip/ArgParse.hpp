#pragma once

#include <string>
#include <vector>

namespace bad_zip {
enum class EngineMode { Compress, Decompress, Unknown };

struct ParsedArgs {
    // Failure state and message
    bool parse_failed = false;
    std::string failure_message;

    // Optional flags
    bool help = false;
    bool quiet = false;

    // Compression/Decompression state and arguments
    EngineMode mode = EngineMode::Unknown;
    std::string archive_name;
    std::vector<std::string> compress_sources;
    std::string decompress_target;
};

ParsedArgs parse_args(int argc, char* argv[]);
}  // namespace bad_zip
