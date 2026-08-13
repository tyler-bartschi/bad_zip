#include <algorithm>
#include <bad_zip/engine/BZipEngine.hpp>
#include <bad_zip/engine/cores/CompressionCore.hpp>
#include <bad_zip/engine/cores/DecompressionCore.hpp>
#include <bad_zip/errors/CompressionError.hpp>
#include <bad_zip/errors/DecompressionError.hpp>
#include <filesystem>
#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
using std::string, std::cout, std::endl, std::cin, std::getline, std::tolower, std::ostringstream;

namespace fs = std::filesystem;

namespace {
constexpr string MODULE_NAME = "BZipEngine";
}

namespace bad_zip {
BZipEngine::BZipEngine(const ParsedArgs& args, const Logger& logger)
    : engine_status(EngineStatus::Unvalidated),
      engine_mode_(args.mode),
      archive_name_(args.archive_name),
      compress_sources_(args.compress_sources),
      decompress_target_(args.decompress_target),
      logger_(logger) {}

void BZipEngine::validate() {
    // check if required files exist based on the mode
    logger_.log(LogLevel::INFO, MODULE_NAME, "Beginning validation...");

    if (engine_mode_ == EngineMode::Unknown) {
        logger_.log(LogLevel::ERROR, MODULE_NAME, "EngineMode is UNKNOWN, cannot proceed");
        engine_status = EngineStatus::Failed;
        return;
    }

    bool is_ready = true;
    if (engine_mode_ == EngineMode::Compress) {
        // compress_sources_ needs to exist, and if archive_name_ exists print a warning and ask
        // before continuing
        try_with_filesystem_catch(is_ready,
                                  [this](bool& ready) { validate_compression_state(ready); });
    } else if (engine_mode_ == EngineMode::Decompress) {
        // archive name must exist as a file, if decompress_target_ exists as a file fail, if
        // it exists as a directory print a warning
        try_with_filesystem_catch(is_ready,
                                  [this](bool& ready) { validate_decompression_state(ready); });
    }

    if (is_ready) {
        logger_.log(LogLevel::INFO, MODULE_NAME, "Validation successful");
        engine_status = EngineStatus::Ready;
    } else {
        logger_.log(LogLevel::ERROR, MODULE_NAME, "Validation failed");
        engine_status = EngineStatus::Failed;
    }
}

void BZipEngine::execute() {
    try {
        if (engine_mode_ == EngineMode::Compress) {
            engine_status = EngineStatus::Running;

            // initialize the compression core then execute
            auto core = CompressionCore(archive_name_, compress_sources_, logger_);
            core.execute();

            engine_status = EngineStatus::Finished;
        } else if (engine_mode_ == EngineMode::Decompress) {
            engine_status = EngineStatus::Running;

            // initialize the decompression core then execute
            auto core = DecompressionCore(archive_name_, decompress_target_, logger_);
            core.execute();

            engine_status = EngineStatus::Finished;
        } else {
            engine_status = EngineStatus::Failed;
            logger_.log(LogLevel::ERROR, MODULE_NAME, "Engine mode in unknown state. Canceling...");
        }
    } catch (const CompressionError& error) {
        engine_status = EngineStatus::Failed;
        ostringstream out;
        out << "A compression error occurred: " << error.what();
        logger_.log(LogLevel::ERROR, MODULE_NAME, out.str());
    } catch (const DecompressionError& error) {
        engine_status = EngineStatus::Failed;
        ostringstream out;
        out << "A decompression error occurred: " << error.what();
        logger_.log(LogLevel::ERROR, MODULE_NAME, out.str());
    } catch (const std::exception& error) {
        engine_status = EngineStatus::Failed;
        ostringstream out;
        out << "An error occurred: " << error.what();
        logger_.log(LogLevel::ERROR, MODULE_NAME, out.str());
    }
}

void BZipEngine::validate_compression_state(bool& is_valid) const {
    logger_.log(LogLevel::INFO, MODULE_NAME, "Compression validation starting...");

    if (fs::is_regular_file(archive_name_) || fs::is_directory(archive_name_)) {
        const string message =
            archive_name_ +
            " already exists. Proceeding will overwrite the existing archive. Proceed? (y/N)";
        confirmation_prompt(message, is_valid);

        if (is_valid) {
            logger_.log(LogLevel::INFO, MODULE_NAME, "Archive overwrite confirmed. Proceeding...");
        } else {
            logger_.log(LogLevel::INFO, MODULE_NAME, "Archive overwrite denied. Canceling...");
            return;
        }
    }

    for (const string& source : compress_sources_) {
        if (!fs::is_regular_file(source) && !fs::is_directory(source)) {
            logger_.log(LogLevel::ERROR, MODULE_NAME, source + " is not a file or directory");
            is_valid = false;
            return;
        }
    }
}

void BZipEngine::validate_decompression_state(bool& is_valid) const {
    logger_.log(LogLevel::INFO, MODULE_NAME, "Decompression validation starting...");

    if (!fs::is_regular_file(archive_name_)) {
        logger_.log(LogLevel::ERROR, MODULE_NAME,
                    archive_name_ + " does not exist or is not a file");
        is_valid = false;
        return;
    }

    if (fs::is_regular_file(decompress_target_) ||
        (fs::exists(decompress_target_) && !fs::is_directory(decompress_target_))) {
        logger_.log(LogLevel::ERROR, MODULE_NAME, decompress_target_ + " is not a directory");
        is_valid = false;
        return;
    }

    if (fs::is_directory(decompress_target_) && !fs::is_empty(decompress_target_)) {
        const string prompt =
            decompress_target_ +
            " is not empty. Proceeding may overwrite matching file names. Proceed? (y/N)";
        confirmation_prompt(prompt, is_valid);
        if (is_valid) {
            logger_.log(LogLevel::INFO, MODULE_NAME,
                        "Directory overwrite confirmed. Proceeding...");
        } else {
            logger_.log(LogLevel::INFO, MODULE_NAME, "Directory overwrite denied. Canceling...");
        }
    }
}

void BZipEngine::try_with_filesystem_catch(bool& is_ready,
                                           const std::function<void(bool&)>& func) const {
    try {
        func(is_ready);
    } catch (const fs::filesystem_error& e) {
        logger_.log(LogLevel::ERROR, MODULE_NAME,
                    "A filesystem error occurred: " + e.code().message());
        is_ready = false;
    }
}

void BZipEngine::confirmation_prompt(const string& prompt, bool& should_proceed) {
    cout << prompt << endl;

    string response;
    getline(cin, response);
    to_lowercase(response);

    should_proceed = response == "yes" || response == "y";
}

void BZipEngine::to_lowercase(string& str) {
    std::ranges::transform(str, str.begin(), [](const unsigned char c) { return tolower(c); });
}
}  // namespace bad_zip
