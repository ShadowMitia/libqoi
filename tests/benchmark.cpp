#include "fmt/core.h"

#include <cstdlib>
#include <vector>
#include <filesystem>
#include <fstream>
#include <thread>
#include <string>

using namespace std::literals;

#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch2/catch.hpp"

#include "qoi.hpp"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_NO_LINEAR
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

std::vector<unsigned char> read_binary(std::filesystem::path const& path) {
  std::ifstream file(path, std::fstream::ate | std::fstream::binary);
  const auto size = file.tellg();
  file.seekg(0);
  std::vector<unsigned char> output;
  output.resize(static_cast<std::size_t>(size));
  if (not file.read(reinterpret_cast<char*>(output.data()), size)) { // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
    return {};
  }
  return output;
}

std::uint64_t Fibonacci(std::uint64_t number) {
    return number < 2 ? 1 : Fibonacci(number - 1) + Fibonacci(number - 2);
}


TEST_CASE("Fibonacci") {
    CHECK(Fibonacci(0) == 1);
    // some more asserts..
    CHECK(Fibonacci(5) == 8);
    // some more asserts..

    // now let's benchmark:
    BENCHMARK("Fibonacci 20") {
        return Fibonacci(20);
    };

    BENCHMARK("Fibonacci 25") {
        return Fibonacci(25);
    };

    BENCHMARK("Fibonacci 30") {
        return Fibonacci(30);
    };

    BENCHMARK("Fibonacci 35") {
        return Fibonacci(35);
    };
}

TEST_CASE("Decode") {

  BENCHMARK_ADVANCED("libqoi")(Catch::Benchmark::Chronometer meter) {

    const auto image_path = std::filesystem::path("qoi_test_images/dice.png");
    
    const auto data = read_binary(image_path);
    
    meter.measure([data]{
      return qoi::decode(data);
    });
  };
  
}
