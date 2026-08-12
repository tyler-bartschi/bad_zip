#pragma once

#include <bad_zip/logging/Logger.hpp>
#include <filesystem>
#include <string>
#include <vector>

namespace bad_zip {
class CompressionCore {
public:
    explicit CompressionCore(const std::string& archive_name,
                             const std::vector<std::string>& compression_sources,
                             const Logger& logger);

    void execute() const;

private:
    const std::filesystem::path archive_path_;
    const std::vector<std::filesystem::path> compression_paths_;
    const Logger& logger_;

    static std::vector<std::filesystem::path> make_paths(const std::vector<std::string>& sources);
};
}  // namespace bad_zip