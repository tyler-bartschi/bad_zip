#pragma once

#include <vector>
using std::vector;
#include <string>
using std::string;

namespace bad_zip {
enum class EngineMode { Compress, Decompress, Unknown };

struct ParsedArgs {
    // Failure state and message
    bool parse_failed = false;
    string failure_message;

    // Optional flags
    bool help = false;
    bool quiet = false;

    // Compression/Decompression state and arguments
    EngineMode mode = EngineMode::Unknown;
    string archive_name;
    vector<string> compress_sources;
    string decompress_target;
};

ParsedArgs parse_args(int argc, char* argv[]);
}  // namespace bad_zip
