#include <bad_zip/errors/DecompressionError.hpp>
#include <stdexcept>
#include <string>
using std::string, std::runtime_error;

namespace bad_zip {
DecompressionError::DecompressionError(const string& message) : runtime_error(message) {}
}  // namespace bad_zip
