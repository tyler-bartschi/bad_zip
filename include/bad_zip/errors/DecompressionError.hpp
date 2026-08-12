#pragma once

#include <stdexcept>
#include <string>

namespace bad_zip {
class DecompressionError : public std::runtime_error {
public:
    explicit DecompressionError(const std::string& message);
};
}  // namespace bad_zip
