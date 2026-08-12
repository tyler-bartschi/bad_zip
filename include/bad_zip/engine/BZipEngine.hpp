#pragma once

#include <bad_zip/ArgParse.hpp>
#include <bad_zip/logging/Logger.hpp>
#include <functional>
#include <string>
#include <vector>

namespace bad_zip {
enum class EngineStatus { Ready, Failed, Running, Unvalidated };

class BZipEngine {
public:
    explicit BZipEngine(const ParsedArgs& args, const Logger& logger);

    EngineStatus engine_status;

    void validate();
    void execute();

private:
    const EngineMode engine_mode_;

    const std::string archive_name_;
    const std::vector<std::string> compress_sources_;
    const std::string decompress_target_;

    const Logger& logger_;

    void validate_compression_state(bool& is_valid) const;
    void validate_decompression_state(bool& is_valid) const;

    void try_with_filesystem_catch(bool& is_ready, const std::function<void(bool&)>& func) const;

    static void confirmation_prompt(const std::string& prompt, bool& should_proceed);
    static void to_lowercase(std::string& str);
};
}  // namespace bad_zip
