#include <catch2/catch_test_macros.hpp>
#include <initializer_list>
#include <vector>
using std::initializer_list, std::vector;

#include <bad_zip/ArgParse.hpp>
using bad_zip::parse_args, bad_zip::ParsedArgs, bad_zip::EngineMode;

namespace {
ParsedArgs parse(initializer_list<const char*> args) {
    vector<char*> argv;

    for (const char* arg : args) {
        argv.push_back(const_cast<char*>(arg));
    }

    return parse_args(static_cast<int>(argv.size()), argv.data());
}
}  // namespace

TEST_CASE("providing only 1 argument fails", "[argparse]") {
    const ParsedArgs result = parse({"bad_zip"});

    REQUIRE(result.parse_failed);
    REQUIRE(result.failure_message == "Not enough arguments provided");
}

TEST_CASE("parse_args sets help to true", "[argparse]") {
    SECTION("using -h alone") {
        const ParsedArgs result = parse({"bad_zip", "-h"});

        REQUIRE(result.help);
        REQUIRE_FALSE(result.parse_failed);
    }

    SECTION("using -h with other arguments") {
        const ParsedArgs result_one = parse({"bad_zip", "-h", "text.bzip", "text.txt"});
        const ParsedArgs result_two = parse({"bad_zip", "-c", "-h", "text.bzip", "text.txt"});
        const ParsedArgs result_three =
            parse({"bad_zip", "-h", "-q", "-c", "text.bzip", "text.txt"});
        const ParsedArgs result_four = parse({"bad_zip", "-c", "hello.bzip", "-h", "text.txt"});
        const ParsedArgs result_five =
            parse({"bad_zip", "-q", "-c", "test.bzip", "test.txt", "-h"});

        for (vector results = {result_one, result_two, result_three, result_four, result_five};
             const ParsedArgs& result : results) {
            REQUIRE(result.help);
            REQUIRE_FALSE(result.parse_failed);
        }
    }

    SECTION("using --help alone") {
        const ParsedArgs result = parse({"bad_zip", "--help"});

        REQUIRE(result.help);
        REQUIRE_FALSE(result.parse_failed);
    }

    SECTION("using --help with other arguments") {
        const ParsedArgs result_one = parse({"bad_zip", "--help", "text.bzip", "text.txt"});
        const ParsedArgs result_two = parse({"bad_zip", "-c", "--help", "text.bzip", "text.txt"});
        const ParsedArgs result_three =
            parse({"bad_zip", "--help", "-q", "-c", "text.bzip", "text.txt"});
        const ParsedArgs result_four = parse({"bad_zip", "-c", "hello.bzip", "--help", "text.txt"});
        const ParsedArgs result_five =
            parse({"bad_zip", "-q", "-c", "test.bzip", "test.txt", "--help"});

        for (vector results = {result_one, result_two, result_three, result_four, result_five};
             const ParsedArgs& result : results) {
            REQUIRE(result.help);
            REQUIRE_FALSE(result.parse_failed);
        }
    }

    SECTION("absence of -h or --help does not set the help flag") {
        const ParsedArgs result = parse({"bad_zip", "-c", "test.txt", "test.txt"});

        REQUIRE_FALSE(result.help);
        REQUIRE_FALSE(result.parse_failed);
    }
}

TEST_CASE("parse_args sets options other than help correctly", "[argparse]") {
    SECTION("using -q or --quiet to set the quiet flag") {
        const ParsedArgs result_one = parse({"bad_zip", "-c", "-q", "test.txt", "test.txt"});
        const ParsedArgs result_two = parse({"bad_zip", "-c", "--quiet", "test.txt", "test.txt"});
        const ParsedArgs result_three = parse({"bad_zip", "-c", "test.txt", "test.txt", "--quiet"});
        const ParsedArgs result_four =
            parse({"bad_zip", "-c", "test.txt", "test.txt", "test.txt", "-q"});

        for (vector results = {result_one, result_two, result_three, result_four};
             const ParsedArgs& result : results) {
            REQUIRE(result.quiet);
            REQUIRE_FALSE(result.parse_failed);
        }
    }

    SECTION("absence of -q or --quiet does not set the quiet flag") {
        const ParsedArgs result = parse({"bad_zip", "-c", "hello.txt", "hello.txt"});
        REQUIRE_FALSE(result.quiet);
        REQUIRE_FALSE(result.parse_failed);
    }

    SECTION("using -c or --compress to set the compress mode") {
        const ParsedArgs result_one = parse({"bad_zip", "test.txt", "test.txt", "--compress"});
        const ParsedArgs result_two = parse({"bad_zip", "test.txt", "test.txt", "-c"});
        const ParsedArgs result_three = parse({"bad_zip", "-c", "test.txt", "test.txt"});
        const ParsedArgs result_four =
            parse({"bad_zip", "--compress", "test.txt", "test.txt", "test.txt"});

        for (vector results = {result_one, result_two, result_three, result_four};
             const ParsedArgs& result : results) {
            REQUIRE(result.mode == EngineMode::Compress);
            REQUIRE_FALSE(result.parse_failed);
        }
    }

    SECTION("using -d or --decompress to set the decompress mode") {
        const ParsedArgs result_one = parse({"bad_zip", "test.txt", "test.txt", "--decompress"});
        const ParsedArgs result_two = parse({"bad_zip", "test.txt", "test.txt", "-d"});
        const ParsedArgs result_three = parse({"bad_zip", "-d", "test.txt", "test.txt"});
        const ParsedArgs result_four = parse({"bad_zip", "--decompress", "test.txt", "test.txt"});

        for (vector results = {result_one, result_two, result_three, result_four};
             const ParsedArgs& result : results) {
            REQUIRE(result.mode == EngineMode::Decompress);
            REQUIRE_FALSE(result.parse_failed);
        }
    }

    SECTION("multiple of the same flag ignored") {
        const ParsedArgs result_one = parse({"bad_zip", "-c", "test.txt", "-c", "test.txt"});
        const ParsedArgs result_two = parse({"bad_zip", "-d", "-d", "-q", "test.txt", "test.txt"});
        const ParsedArgs result_three =
            parse({"bad_zip", "-q", "-q", "--decompress", "test.txt", "test.txt"});

        REQUIRE(result_one.mode == EngineMode::Compress);
        REQUIRE_FALSE(result_one.parse_failed);
        REQUIRE(result_two.mode == EngineMode::Decompress);
        REQUIRE_FALSE(result_two.parse_failed);
        REQUIRE(result_three.quiet);
        REQUIRE_FALSE(result_three.parse_failed);
    }
}

TEST_CASE("parse_args properly handles failure cases", "[argparse]") {
    SECTION("absence of -c or -d keeps the mode as unknown") {
        const ParsedArgs result = parse({"bad_zip", "text.txt", "text.txt"});
        REQUIRE(result.mode == EngineMode::Unknown);
        REQUIRE(result.parse_failed);
        REQUIRE(result.failure_message == "Engine Mode not set. Compress/decompress flag required");
    }

    SECTION("conflicting compress/decompress flags fails") {
        const ParsedArgs result_one = parse({"bad_zip", "-c", "-d"});
        const ParsedArgs result_two = parse({"bad_zip", "-d", "test.txt", "test.txt", "-c"});
        const ParsedArgs result_three =
            parse({"bad_zip", "--compress", "--decompress", "test.txt"});
        const ParsedArgs result_four = parse({"bad_zip", "-c", "--decompress", "test.txt"});

        for (vector results = {result_one, result_two, result_three, result_four};
             const ParsedArgs& result : results) {
            REQUIRE(result.parse_failed);
            REQUIRE(result.failure_message == "Conflicting compress/decompress flags");
        }
    }

    SECTION("only providing one file fails on both modes") {
        const ParsedArgs result_one = parse({"bad_zip", "-c", "test.txt"});
        const ParsedArgs result_two = parse({"bad_zip", "-d", "test.txt"});

        for (vector results = {result_one, result_two}; const ParsedArgs& result : results) {
            REQUIRE(result.parse_failed);
            REQUIRE(result.failure_message == "Not enough filenames or directories provided");
        }
    }

    SECTION("providing multiple target names in decompress mode fails") {
        const ParsedArgs result = parse({"bad_zip", "-d", "compressed.bzip", "target", "target2"});
        REQUIRE(result.parse_failed);
        REQUIRE(result.failure_message == "Too many arguments provided for decompress");
    }
}

TEST_CASE("properly assigns filename/directory arguments", "[argparse]") {
    SECTION("properly assigns a single filename/directory in compress mode") {
        const ParsedArgs result = parse({"bad_zip", "-c", "compressed.bzip", "test.txt"});
        REQUIRE_FALSE(result.parse_failed);
        REQUIRE(result.mode == EngineMode::Compress);
        REQUIRE(result.archive_name == "compressed.bzip");
        REQUIRE(result.compress_sources.size() == 1);
        REQUIRE(result.compress_sources[0] == "test.txt");
        REQUIRE(result.decompress_target.empty());
    }

    SECTION("properly assigns multiple filenames/directories in compress mode") {
        const ParsedArgs result =
            parse({"bad_zip", "-c", "compressed.bzip", "test.txt", "another.txt", "directory"});
        REQUIRE_FALSE(result.parse_failed);
        REQUIRE(result.mode == EngineMode::Compress);
        REQUIRE(result.archive_name == "compressed.bzip");
        REQUIRE(result.compress_sources.size() == 3);
        REQUIRE(result.compress_sources[0] == "test.txt");
        REQUIRE(result.compress_sources[1] == "another.txt");
        REQUIRE(result.compress_sources[2] == "directory");
        REQUIRE(result.decompress_target.empty());
    }

    SECTION("properly assigns filename/directory in decompress mode") {
        const ParsedArgs result = parse({"bad_zip", "-d", "compressed.bzip", "target"});
        REQUIRE_FALSE(result.parse_failed);
        REQUIRE(result.mode == EngineMode::Decompress);
        REQUIRE(result.archive_name == "compressed.bzip");
        REQUIRE(result.decompress_target == "target");
        REQUIRE(result.compress_sources.empty());
    }
}
