#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <filesystem>
#include <fmt/core.h>
#include <fstream>
#include <vector>

#include "qoi.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
  
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

void write_binary(const std::filesystem::path& destination_image, std::vector<unsigned char> pixels) {
  std::ofstream file(destination_image, std::ios::out | std::ios::binary);
  const auto size = static_cast<std::streamsize>(pixels.size());
  if (not file.write(reinterpret_cast<char*>(pixels.data()), size)) { // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
    fmt::print("Couldn't write file\n");
  }
}
  

void write_ppm(const std::filesystem::path& destination_image, std::vector<unsigned char> pixels, std::size_t width, std::size_t height) {
  std::ofstream file(destination_image);
  file << "P6\n" << width << ' ' << height << '\n' << "255\n";
  if (!file.write(reinterpret_cast<char*>(pixels.data()), pixels.size())) { // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
    fmt::print("Couldn't write file\n");
  }
}

auto main(int /* argc */, char* argv[]) -> int {

  std::filesystem::path encode_img{argv[1]};
  std::filesystem::path decode_img{argv[2]};

  if (not(std::filesystem::exists(encode_img) or std::filesystem::exists(decode_img))) {
    fmt::print("Usage: ./qoi encode_img decode_img\n");
    return EXIT_FAILURE;
  }

  // Encode
  {
    const auto image_to_encode = read_binary(encode_img);
    int width{0};
    int height{0};
    int channels{0};
    auto * const data = stbi_load(encode_img.c_str(), &width, &height, &channels, 0);
    if (data == nullptr) {
      fmt::print("Couldn't load {}\n", encode_img.c_str());
      return EXIT_FAILURE;
    }

    const std::size_t size = static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * static_cast<std::size_t>(channels);

    std::vector<unsigned char> orig_pixels;
    orig_pixels.resize(size);
    std::memcpy(orig_pixels.data(), data, size);
    stbi_image_free(data);
  
    const auto encoded_pixels = qoi::encode(orig_pixels, static_cast<std::uint32_t>(width), static_cast<std::uint32_t>(height), static_cast<unsigned char>(channels));
  
    write_binary("test.qoi", encoded_pixels);
    
    {
      const auto image_to_decode = read_binary("test.qoi");
      // fmt::print("image_to_decode {}\n", image_to_decode.size());
      const auto header = qoi::get_header(image_to_decode);
      const auto pixels = qoi::decode(image_to_decode);
      std::vector<unsigned char> pix;
      pix.reserve(static_cast<std::size_t>(header.width * header.height) * header.channels);
      for (std::size_t i = 0; i < pixels.size(); i+=header.channels) {
	pix.push_back(pixels[i + 0]);
	pix.push_back(pixels[i + 1]);
	pix.push_back(pixels[i + 2]);
      }
      write_ppm("test2.ppm", pix, header.width, header.height);
      stbi_write_png("test2.png", static_cast<int>(header.width), static_cast<int>(header.height), header.channels, pixels.data(), static_cast<int>(header.width * header.channels));
    }
  }

  // Decode
  {
    const auto image_to_decode = read_binary(decode_img);
    const auto header = qoi::get_header(image_to_decode);
    const auto pixels = qoi::decode(image_to_decode);
    if (header.channels == 4) {
      std::vector<unsigned char> pix;
      pix.reserve(header.width * header.height * header.channels);
      for (std::size_t i = 0; i < pixels.size(); i+=header.channels) {
	pix.push_back(pixels[i + 0]);
	pix.push_back(pixels[i + 1]);
	pix.push_back(pixels[i + 2]);
      }
      write_ppm("test.ppm", pix, header.width, header.height);
      stbi_write_png("test.png", header.width, header.height, header.channels, pixels.data(), header.width * header.channels);
    } else {
      write_ppm("test.ppm", pixels, header.width, header.height);
      stbi_write_png("test.png", header.width, header.height, header.channels, pixels.data(), header.width * header.channels);
    }
  }
  return EXIT_SUCCESS;
}
