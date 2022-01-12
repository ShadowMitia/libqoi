#include <cstring>
#include <cstddef>
#include <cstdint>

#include "qoi.hpp"

#include <vector>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size) {
  if (size < 4) {
    return 0;
  }
  std::vector<unsigned char> d;
  d.resize(size);
  std::memcpy(d.data(), data, size);
  // volatile to avoid optimizing return away, because we are not using it.
  volatile auto ret = qoi::get_header(d);
  // call with void to avoid unused warning.
  (void)(ret);
  return 0;
}
