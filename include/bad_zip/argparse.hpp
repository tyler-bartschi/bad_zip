#pragma once

#include <vector>
using std::vector;
#include <string>
using std::string;

namespace bad_zip {
enum class EngineMode {
    Compress,
    Decompress,
    Unknown
};

struct ParsedArgs {
    // Failure state and message
    bool parse_failed = false;
    string failure_message;

    // Optional flags
    bool show_help = false;
    bool is_quiet = false;

    // Compression/Decompression state and arguments
    EngineMode mode = EngineMode::Unknown;
    string compressed_file_name;
    vector<string> compress_targets;
    string decompress_target;
};

ParsedArgs parse_args(int argc, char* argv[]);
}
