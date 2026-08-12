#include <bad_zip/engine/BZipEngine.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <vector>
using bad_zip::EngineMode, bad_zip::ParsedArgs, bad_zip::BZipEngine, bad_zip::EngineStatus;
using std::string, std::vector;

namespace fs = std::filesystem;

namespace {
class TempDir {
public:
    TempDir() : path_(make_temp_dir()) {}

    ~TempDir() {
        std::error_code ec;
        fs::remove_all(path_, ec);
    }

    [[nodiscard]] const fs::path& path() const {
        return path_;
    }

private:
    const fs::path path_;

    static fs::path make_temp_dir() {
        const fs::path base = fs::temp_directory_path();

        const auto stamp = std::chrono::steady_clock::now().time_since_epoch().count();

        for (int attempt = 0; attempt < 100; ++attempt) {
            fs::path candidate =
                base / ("catch-test-" + std::to_string(stamp) + "-" + std::to_string(attempt));

            if (std::error_code ec; fs::create_directory(candidate, ec)) {
                return candidate;
            }
        }

        throw std::runtime_error("Could not create temporary directory");
    }
};

ParsedArgs create_args(const string& archive_name, const vector<string>& compress_sources,
                       const string& decompress_target,
                       const EngineMode& mode = EngineMode::Compress) {
    ParsedArgs args;

    args.failure_message = "";
    args.mode = mode;
    args.archive_name = archive_name;
    args.compress_sources = compress_sources;
    args.decompress_target = decompress_target;

    return args;
}
}  // namespace

TEST_CASE("BZipEngine validation failures properly set the EngineMode to Failed", "[BZipEngine]") {
    const auto temp = TempDir();
    const fs::path& base = temp.path();

    std::ostringstream out;
    const auto logger = bad_zip::Logger(false, &out);

    SECTION("EngineMode being UNKNOWN triggers failure") {
        const ParsedArgs args = create_args("", vector<string>(), "", EngineMode::Unknown);
        auto engine = BZipEngine(args, logger);
        engine.validate();

        REQUIRE(engine.engine_status == EngineStatus::Failed);
        REQUIRE(out.str().find("EngineMode is UNKNOWN, cannot proceed"));
    }

    SECTION("<COMPRESS> An already existing archive_name") {}
}
