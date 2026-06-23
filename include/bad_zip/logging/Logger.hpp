#pragma once

#include <ostream>
#include <string>

namespace bad_zip {
enum class LogLevel { DEBUG, INFO, WARN, ERROR };

class Logger {
public:
    explicit Logger(bool quiet, std::ostream* out_override = nullptr);
    void log(const LogLevel& level, const std::string& source, const std::string& message) const;
    static std::string log_level_to_string(const LogLevel& level);

private:
    const bool quiet_;
    std::ostream* out_override_;
};
}  // namespace bad_zip