#include <bad_zip/errors/CompressionError.hpp>
#include <stdexcept>
#include <string>
using std::string, std::runtime_error;

namespace bad_zip {
CompressionError::CompressionError(const string& message) : runtime_error(message) {}
}  // namespace bad_zip
