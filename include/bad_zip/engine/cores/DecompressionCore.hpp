#pragma once

#include <bad_zip/logging/Logger.hpp>
#include <filesystem>
#include <string>

namespace bad_zip {
class DecompressionCore {
public:
    explicit DecompressionCore(const std::string& archive_name,
                               const std::string& decompress_target, const Logger& logger);

    void execute() const;

private:
    const std::filesystem::path archive_path_;
    const std::filesystem::path decompress_path_;
    const Logger& logger_;
};
}  // namespace bad_zip