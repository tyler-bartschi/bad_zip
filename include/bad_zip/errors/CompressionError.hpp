#pragma once

#include <stdexcept>
#include <string>

namespace bad_zip {
class CompressionError : public std::runtime_error {
public:
    explicit CompressionError(const std::string& message);
};
}  // namespace bad_zip
