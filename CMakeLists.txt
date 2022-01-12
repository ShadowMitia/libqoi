cmake_minimum_required(VERSION 3.4)

# A Quite OK Image format implementation written in C++
project(qoi
    VERSION 0.1
    LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 14)

include(${PROJECT_SOURCE_DIR}/cmake/load_config.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/load_tools.cmake)
add_compile_commands_json()

include(${PROJECT_SOURCE_DIR}/cmake/flags.cmake)

conan_get_package(PACKAGE fmt/8.0.1 libpng/1.6.37 catch2/2.13.7 stb/cci.20210713)

add_library(qoi "src/qoi.cpp" "include/qoi/qoi.hpp")
add_library(qoi::qoi ALIAS qoi)
target_include_directories(
    qoi
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/qoi>
    $<INSTALL_INTERFACE:include/qoi>
)
target_compile_features(qoi PUBLIC cxx_std_14)

### TESTS ###
# gtest_add_test(NAME gtest_example SOURCES ./tests/test_examples/gtest_example.cpp)
# catch2_add_test(NAME benchmark SOURCES "tests/benchmark.cpp")
# target_link_libraries(benchmark PRIVATE qoi CONAN_PKG::fmt CONAN_PKG::catch2 CONAN_PKG::stb) # you can link targets from the {NAME}_add_test functions
# target_include_directories(benchmark SYSTEM PRIVATE "${PROJECT_SOURCE_DIR}/external/stb")
# target_compile_features(benchmark PUBLIC cxx_std_20)

catch2_add_test(NAME qoi_test SOURCES tests/qoi_tests.cpp)
target_link_libraries(qoi_test PRIVATE CONAN_PKG::fmt qoi CONAN_PKG::catch2 CONAN_PKG::stb) # you can link targets from the {NAME}_add_test functions
target_include_directories(qoi_test SYSTEM PUBLIC "${PROJECT_SOURCE_DIR}/external/stb")

add_custom_command(TARGET qoi_test POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/tests/qoi_test_images 
    ${CMAKE_CURRENT_BINARY_DIR}/qoi_test_images)

### FUZZING ###
libfuzz_add_test(NAME libfuzz_qoi SOURCES "tests/qoi_fuzz.cpp" ARGS -max_total_time=10)
target_link_libraries(libfuzz_qoi PRIVATE qoi)