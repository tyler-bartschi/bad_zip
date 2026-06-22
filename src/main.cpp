#include <iostream>
#include <string>
#include <string_view>
using std::cout, std::cerr, std::endl, std::string;

#include <bad_zip/ArgParse.hpp>

namespace {
constexpr std::string_view USAGE_MESSAGE =
    "Usage: ./build/debug/bad_zip [options] [archive_name] [target_file_or_directory_name(s)]";

void print_help_message() {
    cout << "bad_zip: A compression and decompression algorithm using Huffman Encoding. Like zip, "
            "just not as efficient"
         << endl;
    cout << USAGE_MESSAGE << endl;
    cout << "\nOptions:" << endl;
    cout << "'-h': help, prints the help message (this message). Also: '--help'" << endl;
    cout << "'-q': enable quiet mode, does not print output messages. Also: '--quiet'" << endl;
    cout << "\n'-c': launch the program in compression mode. Also: '--compress'" << endl;
    cout << "'-d': launch the program in decompression mode. Also: '--decompress'" << endl;
    cout << "\nCompression mode:" << endl;
    cout << "[archive_name] - Provide a name for the generated archive file." << endl;
    cout << "[target_file_or_directory_name(s)] - Provide one or more files or directories to "
            "compress. Directories are automatically recursively traversed"
         << endl;
    cout << "\nDecompression mode:" << endl;
    cout << "[archive_name] - Provide the name or path of an existing bad_zip archive" << endl;
    cout << "[target_file_or_directory_name(s)] - Provide the name of the directory to write the "
            "decompressed contents to."
         << endl;
    cout << "If it does not already exist, this directory will be created. You cannot provide "
            "multiple names to decompress"
         << endl;
    cout << "\nOther notes:" << endl;
    cout << "- '-h' or '--help' overrides all other provided parameters, and will always display "
            "this message"
         << endl;
    cout << "- Either the compression or decompression flag must be provided, neither or both will "
            "generate a failure output"
         << endl;
}

void handle_parse_failure(const string& failure_message) {
    cerr << "Argument parsing failed with the following failure: " << failure_message << endl;
    cerr << USAGE_MESSAGE << endl;
    cerr << "Run with the -h or --help flag for more information" << endl;
}
}  // namespace

int main(const int argc, char* argv[]) {
    const bad_zip::ParsedArgs parsed_args = bad_zip::parse_args(argc, argv);

    if (parsed_args.help) {
        // print help message
        print_help_message();
        return 0;
    }
    if (parsed_args.parse_failed) {
        // handle failure
        handle_parse_failure(parsed_args.failure_message);
        return 1;
    }
    return 0;
}
