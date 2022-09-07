#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

#include "qoi.hpp"

TEST_CASE("Header : get_header", "[header]") {
  std::vector<unsigned char> test_header{0x71, 0x6f, 0x69, 0x66, 0x00, 0x00, 0x03, 0x00, 0x0, 0x0, 0x2, 0x0, 0x3, 0x0, 0xfe, 0x74};

  const auto head = qoi::get_header(test_header);

  REQUIRE(head.width == 768);
  REQUIRE(head.height == 512);
  REQUIRE(head.channels == 3);
  REQUIRE(head.colorspace == 0);  
}

TEST_CASE("Header : is_valid", "[header]") {
  std::vector<unsigned char> test_header{0x71, 0x6f, 0x69, 0x65, 0x00, 0x00, 0x03, 0x00, 0x0, 0x0, 0x2, 0x0, 0x3, 0x0, 0xfe, 0x74};

  REQUIRE(qoi::is_valid(test_header) == false); 
}

TEST_CASE("Padding : truncat", "[padding]") {
    std::vector<unsigned char> test_header{0x71, 0x6f, 0x69, 0x66, 0x00, 0x00, 0x03, 0x00, 0x0, 0x0, 0x2, 0x0, 0x3, 0x0, 0xfe};

    REQUIRE(qoi::is_valid(test_header) == false);
}

TEST_CASE("Decode dice.qoi", "[decode]") {

  const std::string ref_img = "qoi_test_images/dice.png";
  const std::string decode_img = "qoi_test_images/dice.qoi";
  
  int width{0};
  int height{0};
  int channels{0};
  auto * const data = stbi_load(ref_img.c_str(), &width, &height, &channels, 0);
  
  const auto size = static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * static_cast<std::size_t>(channels);
  
  std::vector<unsigned char> d(data, data + size);
  stbi_image_free(data);
  
  const auto dice_data = qoi::utils::read_binary(decode_img);
  const auto image = qoi::decode(dice_data);
  const auto header = qoi::get_header(dice_data);
  
  REQUIRE(header.width == 800);
  REQUIRE(header.height == 600);
  REQUIRE(image[3] == 0x0);
  REQUIRE(d == image);
}

TEST_CASE("Encode", "[encode]") {
  
  const std::string encode_img = "qoi_test_images/dice.png";
  const std::string ref_img = "qoi_test_images/dice.qoi";
  
  int width{0};
  int height{0};
  int channels{0};
  auto * const data = stbi_load(encode_img.c_str(), &width, &height, &channels, 0);
  
  const auto size = static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * static_cast<std::size_t>(channels);
  
  std::vector<unsigned char> d;
  d.resize(size);
  std::memcpy(d.data(), data, size);
  stbi_image_free(data);
  
  const auto image = qoi::encode(d, width, height, channels);
  const auto header = qoi::get_header(image);

  const auto ref = qoi::utils::read_binary(ref_img);
  
  REQUIRE(header.width == 800);
  REQUIRE(header.height == 600);
  REQUIRE(ref == image);
}
