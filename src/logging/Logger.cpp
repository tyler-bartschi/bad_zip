#include <bad_zip/logging/Logger.hpp>
#include <iostream>
#include <string>
using std::ostream, std::cout, std::cerr, std::endl, std::string;

namespace bad_zip {
Logger::Logger(const bool quiet, ostream* out_override)
    : quiet_(quiet), out_override_(out_override) {}

void Logger::log(const LogLevel& level, const std::string& source,
                 const std::string& message) const {
    if (quiet_ &&
        (level == LogLevel::DEBUG || level == LogLevel::INFO || level == LogLevel::WARN)) {
        return;
    }

    bool use_error = false;
    if (level == LogLevel::WARN || level == LogLevel::ERROR || level == LogLevel::FATAL) {
        use_error = true;
    }

    ostream& out = out_override_ != nullptr ? *out_override_ : use_error ? cerr : cout;

    out << "\033[1m[" << log_level_to_string(level) << "]\033[0m <" << source << "> " << message
        << endl;
}

std::string Logger::log_level_to_string(const LogLevel& level) {
    switch (level) {
        case LogLevel::DEBUG:
            return "DEBUG";
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARN:
            return "WARN";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
    }
}
}  // namespace bad_zip