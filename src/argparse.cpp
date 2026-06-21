#include <iostream>
#include <string>
#include <string_view>
#include <vector>
using std::cout, std::endl, std::string, std::string_view, std::vector;

#include <bad_zip/argparse.hpp>
using bad_zip::EngineMode, bad_zip::ParsedArgs;

namespace {
constexpr string_view CONFLICT_MESSAGE = "Conflicting compress/decompress flags";

bool catch_conflicting_mode(const ParsedArgs& result, const char& flag) {
    // Returns false if there is not conflicting mode flags, true otherwise
    if (result.mode == EngineMode::Unknown) {
        return false;
    }
    if (result.mode == EngineMode::Compress && flag == 'c') {
        return false;
    }
    if (result.mode == EngineMode::Decompress && flag == 'd') {
        return false;
    }
    return true;
}

void process_file_and_directory_args(ParsedArgs& result, const vector<string>& names) {
    result.archive_name = names[0];

    if (result.mode == EngineMode::Decompress && names.size() == 2) {
        result.decompress_target = names[1];
    } else if (result.mode == EngineMode::Compress) {
        for (size_t i = 1; i < names.size(); ++i) {
            result.compress_sources.push_back(names[i]);
        }
    } else if (result.mode == EngineMode::Unknown) {
        result.parse_failed = true;
        result.failure_message = "Engine Mode not set. Compress/decompress flag required";
    } else {
        result.parse_failed = true;
        result.failure_message = "Too many arguments provided for decompress";
    }
}
}  // namespace

namespace bad_zip {
ParsedArgs parse_args(int argc, char* argv[]) {
    ParsedArgs result;

    if (argc < 2) {
        result.parse_failed = true;
        result.failure_message = "Not enough arguments provided";
        return result;
    }

    vector<string> found_names;

    for (int i = 1; i < argc; ++i) {
        if (string arg = argv[i]; arg.starts_with('-') && !arg.starts_with("--")) {
            // option flag
            switch (arg[1]) {
                case 'q':
                    // quiet mode
                    result.quiet = true;
                    break;
                case 'h':
                    // print help
                    result.help = true;
                    break;
                case 'c':
                    // compress mode
                    if (catch_conflicting_mode(result, 'c')) {
                        result.parse_failed = true;
                        result.failure_message = CONFLICT_MESSAGE;
                    } else {
                        result.mode = EngineMode::Compress;
                    }
                    break;
                case 'd':
                    // decompress mode
                    if (catch_conflicting_mode(result, 'd')) {
                        result.parse_failed = true;
                        result.failure_message = CONFLICT_MESSAGE;
                    } else {
                        result.mode = EngineMode::Decompress;
                    }
                    break;
                default:
                    // Unknown flag
                    cout << "Unknown flag: " << arg[1] << ". Skipping..." << endl;
                    break;
            }
        } else if (arg == "--help") {
            // long form help flag
            result.help = true;
        } else if (arg == "--quiet") {
            // long form quiet flag
            result.quiet = true;
        } else if (arg == "--compress") {
            // long form compress flag
            if (catch_conflicting_mode(result, 'c')) {
                result.parse_failed = true;
                result.failure_message = CONFLICT_MESSAGE;
            } else {
                result.mode = EngineMode::Compress;
            }
        } else if (arg == "--decompress") {
            // long form decompress flag
            if (catch_conflicting_mode(result, 'd')) {
                result.parse_failed = true;
                result.failure_message = CONFLICT_MESSAGE;
            } else {
                result.mode = EngineMode::Decompress;
            }
        } else if (arg.starts_with("--")) {
            // Unknown long form flag, skip
            cout << "Unknown flag: " << arg.substr(2) << ". Skipping...";
        } else {
            // found a filename, add to the vector
            found_names.push_back(arg);
        }

        if (result.parse_failed || result.help) {
            return result;
        }
    }

    if (found_names.size() < 2) {
        result.parse_failed = true;
        result.failure_message = "Not enough filenames or directories provided";
    } else {
        process_file_and_directory_args(result, found_names);
    }

    return result;
}
}  // namespace bad_zip